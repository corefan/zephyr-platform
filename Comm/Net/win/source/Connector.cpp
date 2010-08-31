#include "Connector.h"
#include "ConnectionMgr.h"
#include "winsock2.h"

namespace Zephyr
{
TInt32 CConnector::Init(TInt32 maxPendingConnections,IfNetApp *pIfNetApp,HANDLE completionPort)
{
    m_pendingConnections = 0;
    m_connected          = 0;
    m_failed             = 0;
    if (maxPendingConnections > FD_SETSIZE*3)
    {
        return OUT_OF_RANGE;
    }
    m_pList = NULL;
    m_pIfNetApp = pIfNetApp;
    m_hCompletionPort = completionPort;
    return SUCCESS;
}

TInt32 CConnector::Run(const TUInt32 runCnt)
{
    int usedCnt = 0;
    if (!m_pList)
    {
        return SUCCESS;
    }
    fd_set wset,exceptSet;
    FD_ZERO(&wset);
    FD_ZERO(&exceptSet);
    CConnection *pList = m_pList;
    int nfds = 0;
    while ((pList) && (usedCnt < runCnt))
    {
        CConnection *pCheckBegin = pList;
        CConnection *pCheckEnd  = NULL;
        while (pList)
        {
            FD_SET(pList->GetSocket(),&wset);
            pCheckEnd = pList;
            pList = pList->GetNext();
            nfds ++;
            if (nfds >= FD_SETSIZE)
            {
                break;
            }
        }
        
        exceptSet = wset;
        struct timeval tm;
        tm.tv_sec=0;
        tm.tv_usec=0;
        nfds = select(nfds, NULL,&wset,&exceptSet,&tm);
        //TInt32 usedCnt = 0;
        switch (nfds) 
        {
        case -1:
            {
                int errCode = WSAGetLastError();
                return -errCode;
            }
        case 0: // nothing happened!
            break;
        default:
            {
                CConnection *pCheck = pCheckBegin;
                usedCnt += nfds;
                //pList = m_pList;
                while ((nfds))
                {
                    TUInt32 state = 0;
                    SOCKET sock = pCheck->GetSocket();
                    CConnection *pNext = pCheck->GetNext();
                    if (FD_ISSET(sock,&wset))
                    {
                        
                        nfds --;
                        state = connection_is_established;
                    }
                    if (FD_ISSET(sock,&exceptSet))
                    {
                        nfds --;
                        state = connection_is_aborted;
                    }
                    switch  (state)
                    {
                    case connection_is_established:
                        {
                            TInt32 result = m_pIfNetApp->OnConnected(pCheck->GetConnectionIdx(),pCheck->GetAppData());
                            if (SUCCESS != result)
                            {
                                pCheck->Disconnect();
                            }
                            if (pCheck == m_pList)
                            {
                                m_pList = pCheck->GetNext();
                            }
                            pCheck->Detach();
                            m_pendingConnections --;
                            m_connected ++;
                            HANDLE h = CreateIoCompletionPort((HANDLE) sock, m_hCompletionPort, (ULONG_PTR)(pCheck), 0);
                            if (h != m_hCompletionPort)
                            {
                                pCheck->Disconnect();
                            }
                            pCheck->ConnectionIsEstablished();
                            usedCnt ++;
                        }
                        break;
                    case connection_is_aborted:
                        {
                            TInt32 result = m_pIfNetApp->OnNetError(pCheck->GetConnectionIdx(),pCheck->GetAppData());
                            if (pCheck == m_pList)
                            {
                                m_pList = pCheck->GetNext();
                            }
                            pCheck->Detach();
                            m_pendingConnections --;
                            m_failed ++;
                            usedCnt ++;
                        }
                        break;
                    default:
                        {
                            //do nothing!
                        }
                    }
                    if (pCheck == pCheckEnd)
                    {
                        break;
                    }
                    pCheck = pNext;
                }
            }
        }
    }
    
	return usedCnt;
}
    

TInt32 CConnector::Connect(CConnection *pConnection)
{
    if (IsListFull())
    {
        return OUT_OF_MEM;
    }
    SOCKET socket = NULL;
    if((socket=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED))==-1)
    {
        return OUT_OF_MEM;
    }
    pConnection->SetSocket(socket);
    if (pConnection->SetSocketOptions() != SUCCESS)
    {
        closesocket(socket);
        return FAIL;
    }
    SOCKADDR_IN remoteAdd;
    SecureZeroMemory(&remoteAdd,sizeof(sockaddr));
    
    
    SOCKADDR_IN myAdd;
    SecureZeroMemory(&myAdd,sizeof(SOCKADDR_IN));
    myAdd.sin_port = htons(pConnection->GetConPair()->GetMyPort());

	// Fill in the rest of the address structure
	myAdd.sin_family = AF_INET;
	myAdd.sin_addr.s_addr = pConnection->GetConPair()->GetMyIp();

	// bind our name to the socket
	int ret = bind(socket, 
		(LPSOCKADDR)&myAdd, 
		sizeof(struct sockaddr));
    if(ret   ==   SOCKET_ERROR)   
    {   
        closesocket(socket);   
        return FAIL;
    }
    
    remoteAdd.sin_family=AF_INET;
    remoteAdd.sin_addr.s_addr = pConnection->GetConPair()->GetRemoteIp();
    remoteAdd.sin_port = htons(((short)pConnection->GetConPair()->GetRemotePort()));
    
    ret = connect(socket,(struct sockaddr *)&remoteAdd,sizeof(sockaddr));
    if(ret == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if (WSAEWOULDBLOCK == error)
        {
            AddToPendingList(pConnection);
            return SUCCESS;
        }
        else
        {
            return (-ret);
        }
    }
    else
    {
        //?
        return ret;
    }
}



TInt32 CConnector::StopConnecting(CConnection *pConnection)
{
    
    if (pConnection->GetConnectionState() == connection_is_trying)
    {
        if (m_pList == pConnection)
        {
            m_pList = pConnection->GetNext();
            pConnection->Detach();
        }
        m_pendingConnections --;
        pConnection->SetConnectionState(connection_is_not_in_use);
        return SUCCESS;
    }
    return ITEM_NOT_FOUND_IN_LIST;
}
void CConnector::AddToPendingList(CConnection *pConnection)
{
    m_pendingConnections ++;
    pConnection->AttachToHead(m_pList);
    m_pList = pConnection;
    pConnection->SetConnectionState(connection_is_trying);
}

}


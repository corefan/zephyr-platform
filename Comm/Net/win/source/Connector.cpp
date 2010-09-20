#include "Connector.h"
#include "ConnectionMgr.h"
#include "winsock2.h"
#include "SysMacros.h"
namespace Zephyr
{


CConnector::CConnector()
{
    m_pendingConnections        = 0;
    m_maxPendingConnection      = 0;
    m_pList                     = 0;
    m_connected                 = 0;
    m_failed                    = 0;
    //IfNetApp        *m_pIfNetApp;
    m_hCompletionPort           = NULL;

    m_pConnectionPool           = NULL;;

    m_pCryptorFactory           = NULL;;

    m_pParserFactory            = NULL;
}

CConnector::~CConnector()
{

}

TInt32 CConnector::Init(TInt32 maxPendingConnections,HANDLE completionPort,ItemClassPool<CConnection> *pConnectionPool,IfParserFactory *pParserFactory,IfCryptorFactory *pCryptorFactory)
{
    m_maxPendingConnection = maxPendingConnections;
    m_connected          = 0;
    m_failed             = 0;
    if (maxPendingConnections > FD_SETSIZE*3)
    {
        return OUT_OF_RANGE;
    }
    m_pList = NULL;
    m_hCompletionPort = completionPort;
    m_pParserFactory  = pParserFactory;
    m_pCryptorFactory = pCryptorFactory;
    m_pConnectionPool = pConnectionPool;
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
                            m_pendingConnections --;
                            m_connected ++;
                            if (pCheck == m_pList)
                            {
                                m_pList = pCheck->GetNext();
                            }
                            //先解开连接
                            pCheck->Detach();
                            
                            HANDLE h = CreateIoCompletionPort((HANDLE) sock, m_hCompletionPort, (ULONG_PTR)(pCheck), 0);
                            TInt32 result;
                            if (h != m_hCompletionPort)
                            {
                                result = FAIL;
                            }
                            else
                            {
                                result = pCheck->OnConnected();
                            }
                            if (SUCCESS > result)
                            {
                                pCheck->CloseConnection();
                                pCheck->OnDisconnected();
                                m_pConnectionPool->ReleaseConnection(pCheck);
                            }
                            //pCheck->ConnectionIsEstablished();
                            usedCnt ++;
                        }
                        break;
                    case connection_is_aborted:
                        {
                            if (pCheck == m_pList)
                            {
                                m_pList = pCheck->GetNext();
                            }
                            pCheck->Detach();
                            pCheck->CloseConnection();
                            pCheck->OnDisconnected();
                            m_pConnectionPool->ReleaseConnection(pCheck);
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

TInt32 CConnector::Connect(CConPair *pPair,IfConnectionCallBack *pAppCallBack)
{
    CConnection *pNew = m_pConnectionPool->GetConnection();
    if(!pNew)
    {
        return OUT_OF_MEM;
    }
    
    SOCKET socket = SOCKET_ERROR;
    
    if((socket=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED))==-1)
    {
        //m_socket = NULL;
        return OUT_OF_MEM;
    }
    
//     if (SetSocketOptions() != SUCCESS)
//     {
//         closesocket(socket);
//         return FAIL;
//     }
    SOCKADDR_IN remoteAdd;
    SecureZeroMemory(&remoteAdd,sizeof(sockaddr));

    SOCKADDR_IN myAdd;
    SecureZeroMemory(&myAdd,sizeof(SOCKADDR_IN));
    myAdd.sin_port = htons(pPair->GetMyPort());

    // Fill in the rest of the address structure
    myAdd.sin_family = AF_INET;
    myAdd.sin_addr.s_addr = pPair->GetMyIp();

    // bind our name to the socket
    TInt32 ret = bind(socket, 
        (LPSOCKADDR)&myAdd, 
        sizeof(struct sockaddr));
    if(ret   ==   SOCKET_ERROR)   
    {   
        closesocket(socket);   
        return FAIL;
    }

    remoteAdd.sin_family=AF_INET;
    remoteAdd.sin_addr.s_addr = pPair->GetRemoteIp();
    remoteAdd.sin_port = htons(pPair->GetRemotePort());
    
    //没有指定本地IP和本地port,给补上
    if ((0 == pPair->GetMyPort()) || (INADDR_ANY == pPair->GetMyIp()))
    {
        sockaddr_in myAdd;
        int     myNameLen = sizeof(sockaddr_in);
        ret = getsockname(socket,(sockaddr *)&myAdd,&myNameLen);;
        if (ret == SOCKET_ERROR)
        {
            return (-((TInt32)WSAGetLastError()));
        }
        pPair->Init(pPair->GetRemoteIp(),myAdd.sin_addr.s_addr,pPair->GetRemotePort(),ntohs(myAdd.sin_port));
    }
    
    IfParser *pParser;
    TUInt32 connectionIdx = pNew->GetConnectionIdx();
    if (m_pParserFactory)
    {
        pParser = m_pParserFactory->GetParser(pPair,connectionIdx);
    }
    else
    {
        pParser = NULL;
    }
    IfCryptor* pCryptor;
    if (m_pCryptorFactory)
    {
        pCryptor = m_pCryptorFactory->GetCryptor(pPair,connectionIdx);
    }
    else
    {
        pCryptor = NULL;
    }
    
    ret = pNew->Init(socket,pPair,pAppCallBack,pParser,pCryptor);
    
    if(ret   ==   SOCKET_ERROR)   
    {   
        closesocket(socket);
        m_pConnectionPool->ReleaseConnection(pNew);
        return FAIL;
    }
    
    ret = connect(socket,(struct sockaddr *)&remoteAdd,sizeof(sockaddr));
    
    if(ret == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if (WSAEWOULDBLOCK == error)
        {
            AddToPendingList(pNew);
            return WSAEWOULDBLOCK;
        }
        else
        {
            return (-ret);
        }
    }
    else
    {
        //直接成功了？
        OnConnectionEstablish(pNew);
        return ret;
    }
}


void CConnector::AddToPendingList(CConnection *pConnection)
{
    m_pendingConnections ++;
    pConnection->Attach(m_pList);
    m_pList = pConnection;
    pConnection->OnTryConnecting();
}

void CConnector::OnConnectionEstablish(CConnection *pConnection)
{
    pConnection->OnConnected();
}


}


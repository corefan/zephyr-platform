#include "ConnectionMgr.h"




namespace Zephyr
{


CConnectionMgr::CConnectionMgr(IfNetApp *pApp,IfTaskMgr *pTaskMgr)
{
    m_pApp = pApp;
    m_pTaskMgr = pTaskMgr;
    m_lastRunTo = 0;
    
}

TInt32 CConnectionMgr::Init(TUInt32 maxConnectionNum,TUInt32 buffSize)
{
    m_pConnectionPool = new CConnection[maxConnectionNum];
    if (!m_pConnectionPool)
    {
        return OUT_OF_MEM;
    }
    m_pFree = NULL;
    m_pUsed = NULL;
    for (TUInt32 i=0;i<maxConnectionNum;i++)
    {
        m_pConnectionPool[i].AttachToHead(m_pFree);
        m_pConnectionPool[i].OnInit();
        int ret = m_pConnectionPool[i].OnCreate(i,buffSize);
        if (ret < SUCCESS)
        {
            return ret;
        }
        m_pFree = (m_pConnectionPool + i);
    }
    m_pListeners = NULL;
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);	
    if (result != SUCCESS)
    {
        int errCode = GetLastError();
        return -errCode;
    }
    m_maxConnectionNum = maxConnectionNum;

    SOCKET s;	

    //
    // First open a temporary socket that we will use to create the
    // completion port.  In NT 3.51 it will not be necessary to specify
    // the FileHandle parameter of CreateIoCompletionPort()--it will
    // be legal to specify FileHandle as NULL.  However, for NT 3.5
    // we need an overlapped file handle.
    //

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if ( s == INVALID_SOCKET ) 
    {	
        return OUT_OF_MEM;
    }


    // Create the completion port that will be used by all the workers
    // threads.
    m_hCompletionPort = CreateIoCompletionPort( (HANDLE)s, NULL, 0, 0 );
    if ( m_hCompletionPort == NULL ) 
    {
        closesocket( s );
        return FAIL;
    }

    closesocket(s);


    for (TUInt32 i=0;i<NR_OF_NET_WORKER;i++)
    {
        m_netWorkers[i].Init(m_maxConnectionNum,m_pConnectionPool,m_hCompletionPort);
        int ret = m_pTaskMgr->AddTask((m_netWorkers+i),normal_task);
        if (ret < SUCCESS)
        {
            return ret;
        }
    }
    
    int ret = m_connector.Init(128,m_pApp,m_hCompletionPort);
    if (ret < SUCCESS)
    {
        return ret;
    }
    return SUCCESS;
}

CConPair *CConnectionMgr::GetConnectionInfo(TUInt32 connectionIdx)
{
    if (connectionIdx < m_maxConnectionNum)
    {
        if (TRUE != m_pConnectionPool[connectionIdx].IsConnected())
        {
            return NULL;
        }
        return m_pConnectionPool[connectionIdx].GetConPair();
    }
    return NULL;
}



TInt32 CConnectionMgr::SendMsg(TInt32 toConnectionIdx,TUChar *pMsg,TUInt32 msgLen)
{
    if (toConnectionIdx < m_maxConnectionNum)
    {
        if (TRUE != m_pConnectionPool[toConnectionIdx].IsConnected())
        {
            return CONNECTION_NOT_ESTABLISHED;
        }
        return m_pConnectionPool[toConnectionIdx].AppSend(pMsg, msgLen);
    }
    return OUT_OF_RANGE;
}

TInt32 CConnectionMgr::Connect(TChar *pRemoteIp,TChar *pMyIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData)
{
    TUInt32 remoteIp = inet_addr(pRemoteIp);
    TUInt32 myIp = inet_addr(pMyIp);
    return Connect(remoteIp,myIp,remotePort, myPort,  pAppData);
}

TInt32 CConnectionMgr::Connect(TUInt32 remoteIp,TUInt32 myIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData)
{
    CConPair pari;
    CConnection *pNew = GetConnection();
    if(!pNew)
    {
        return OUT_OF_MEM;
    }
    TInt32 result = pNew->OnInit();
    result = pNew->Init(remoteIp,myIp,remotePort,myPort,connection_is_postive,pAppData,this);
    if (result != SUCCESS)
    {
        pNew->OnFinal();
        ReleaseConnection(pNew);
        return result;
    }
    result = m_connector.Connect(pNew);
    if (result != SUCCESS)
    {
        pNew->OnFinal();
        ReleaseConnection(pNew);
        return result;
    }
    return SUCCESS;
}

TInt32 CConnectionMgr::Run(TUInt32 runCnt)
{
    TInt32 usedCnt = 0;
    CListener *pList = m_pListeners;
    while (pList)
    {
        TInt32 hasConnection = pList->HasNewConnection();
        for (TInt32 i=0;i<hasConnection;i++)
        {
            SOCKET sock = pList->Accept();
            if (SOCKET_ERROR == sock)
            {
                continue;
            }
            else
            {
                CConnection *p = GetConnection();
                if (!p)
                {
                    //directly close it.
                    LINGER lingerStruct;
                    lingerStruct.l_onoff = 1;
                    lingerStruct.l_linger = 1;
                    //use api's orignal types
                    setsockopt(sock, SOL_SOCKET, SO_LINGER,
                              (char *)&lingerStruct, sizeof(lingerStruct));

                    closesocket(sock);
                    break;
                }
                
                p->SetSocketOnAccept(sock,this);
                
                HANDLE h = CreateIoCompletionPort((HANDLE) sock, m_hCompletionPort, (ULONG_PTR)(p), 0);
                if (h != m_hCompletionPort)
                {
                    p->Disconnect();
                }
                p->OnConnected();
                
                TInt32 ret = m_pApp->OnConnected(p->GetConnectionIdx(), pList->GetAppData());
                if (ret < SUCCESS)
                {
                    p->Disconnect();
                   
                    ReleaseConnection(p);
                }
                
            }
        }
        pList = pList->GetNext();
    }
    if (usedCnt >= runCnt)
    {
        return usedCnt;
    }
    usedCnt += m_connector.Run((runCnt-usedCnt));
    TUInt32 i=0;
    TUInt32 idx = m_lastRunTo;
    for (;i<m_maxConnectionNum;i++)
    {
        TUInt32 connEvent = m_pConnectionPool[idx].GetEvent();
        if (connEvent != event_connection_nothing)
        {
            usedCnt ++;
            switch(connEvent)
            {
            case event_connection_broken:
                {
                    m_pApp->OnDissconneted(idx,m_pConnectionPool[idx].GetAppData());
                    ReleaseConnection(m_pConnectionPool + idx);
                    break;
                }
            case event_connection_has_new_data:
                {
                    m_pConnectionPool[idx].Callback(m_pApp);
                    //m_pApp->OnRecv()
                }
                break;
            }
            if (usedCnt > runCnt)
            {
                break;
            }
        }
        idx = (idx + 1)%m_maxConnectionNum;
        //usedCnt += m_pConnectionPool[i].Routine();
    }
    m_lastRunTo = (m_lastRunTo + i)%m_maxConnectionNum;
    return usedCnt;
}
TInt32 CConnectionMgr::Disconnect(TUInt32 connectionIdx)
{
    if (connectionIdx > m_maxConnectionNum)
    {
        return OUT_OF_RANGE;
    }
    switch (m_pConnectionPool[connectionIdx].GetConnectionState())
    {
        
        case connection_is_not_in_use:
        {
            return CONNECTION_NOT_ESTABLISHED;
        }
        break;
        case connection_is_trying:
        {
            m_pConnectionPool[connectionIdx].Disconnect();
            return SUCCESS;
        }
        break;
        case connection_is_established:
        {
            m_pConnectionPool[connectionIdx].Disconnect();
            return SUCCESS;
        }
        break;
        case connection_is_broken:
        {
            return SUCCESS;
        }
        break;
        default:
        {
            return CONNECTION_IN_WRONG_STATE;
        }
    }
}
TInt32 CConnectionMgr::SetAppData(TUInt32 connectionIdx,void *pAppData)
{
    if (connectionIdx < m_maxConnectionNum)
    {
        m_pConnectionPool[connectionIdx].SetAppData(pAppData);
        return SUCCESS;
    }
    return OUT_OF_RANGE;
}
TInt32 CConnectionMgr::Listen(TUInt32 ip,TUInt16 port,TUInt16 maxConnection,void *pAppData)
{
    CListener *p = new CListener();
    if (NULL == p)
    {
        return OUT_OF_RANGE;
    }
    p->Init(ip, port, maxConnection,pAppData);
    p->AttachToList(m_pListeners);
    m_pListeners = p;
    return NULL;
}


CConnection *CConnectionMgr::GetConnection()
{
    CConnection *pResult = NULL;
    if (m_pFree)
    {
        pResult = m_pFree;
        m_pFree = m_pFree->GetNext();
        pResult->Detach();
        pResult->OnInit();
    }
    return pResult;
}
void CConnectionMgr::ReleaseConnection(CConnection *pConnection)
{
    pConnection->OnFinal();
    pConnection->AttachToHead(m_pFree);
    m_pFree = pConnection;
}

TInt32 CConnectionMgr::AddNetEvent(TIOEvent &events)
{
    return m_netEvents.WriteData((TUChar*)&events,sizeof(TIOEvent));
}

TInt32 CConnectionMgr::AddAppEvent(TIOEvent &event)
{
    return m_appEvents.WriteData((TUChar*)&event,sizeof(TIOEvent));
}

}

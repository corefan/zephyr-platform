#include "ConnectionMgr.h"




namespace Zephyr
{


CConnectionMgr::CConnectionMgr()
{

    m_pCryptorFactory = NULL;
    m_pListeners      = NULL;
    //m_lastRunTo = 0;
    
}

CConnectionMgr::~CConnectionMgr()
{
    //Final();
}

TInt32 CConnectionMgr::Init(TUInt32 maxConnectionNum,IfTaskMgr *pTaskMgr,IfParserFactory* pParserFactory,IfCryptorFactory *pIfCryptorfactory,TUInt32 buffSize)
{
    m_conncectionPool.Init(maxConnectionNum,buffSize);
    
    NEW(m_pBuff,TUChar,buffSize);
    if (!m_pBuff)
    {
        return OUT_OF_MEM;
    }
    m_buffSize = buffSize;
    
    m_pListeners = NULL;
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);	
    if (result != SUCCESS)
    {
        int errCode = GetLastError();
        return -errCode;
    }

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
        m_netWorker.Init(m_hCompletionPort,&m_conncectionPool,&m_netEventQueues,buffSize);
        int ret = pTaskMgr->AddTask(&m_netWorker,normal_task);
        if (ret < SUCCESS)
        {
            return ret;
        }
    }
    
    int ret = m_connector.Init(128,m_hCompletionPort,&m_conncectionPool,m_pParserFactory,m_pCryptorFactory);
    if (ret < SUCCESS)
    {
        return ret;
    }
    m_pParserFactory = pParserFactory;
    m_pCryptorFactory = pIfCryptorfactory;
    //pTaskMgr->AddTask(this);
    return SUCCESS;
}

void  CConnectionMgr::Final()
{
    m_netWorker.Final();
    while (m_pListeners)
    {
        CListener *pNext = m_pListeners->GetNext();
        m_pListeners->Final();
        delete m_pListeners;
        m_pListeners = pNext;
    }
    
    
    delete m_pParserFactory;
    m_pParserFactory  = NULL;
    delete m_pCryptorFactory;
    m_pCryptorFactory = NULL;
}

TInt32 CConnectionMgr::Connect(const TChar *pRemoteIp,const TChar *pMyIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData)
{
    if (m_connector.IsListFull())
    {
        return IF_NET_ERROR_CODE_TOO_MANY_PENDING_CONNECTIONGS;
    }
    TUInt32 remoteIp = inet_addr(pRemoteIp);
    TUInt32 myIp = inet_addr(pMyIp);
    CConPair pair;
    pair.Init(remoteIp,myIp,remotePort,myPort);
    return m_connector.Connect(&pair,(IfConnectionCallBack*)pAppData);
}


TInt32 CConnectionMgr::Run(TUInt32 runCnt)
{
    TInt32 usedCnt = 0;
    CListener *pList = m_pListeners;
    
    while (pList)
    {
        usedCnt += pList->Run(runCnt-usedCnt);
        if (usedCnt > runCnt)
        {
            return usedCnt;
        }
        pList = pList->GetNext();
    }
    usedCnt += m_connector.Run((runCnt-usedCnt));
    TIOEvent *pEvent = m_netEventQueues.GetNetEvent();
    TIOEvent event = *pEvent;
    m_netEventQueues.ConfirmHandleNetEvent(pEvent);
    CConnection *pConnection = m_conncectionPool.GetConectionByIdx(pEvent->m_connectionIdx);
    if (pConnection)
    {
        //由应用层调用，如果返回-1，则表示需要释放连接,把链接close,并且放回connectionPool
        TInt32 ret = pConnection->AppRoutine(m_pBuff,m_buffSize);
        if (ret < SUCCESS)
        {
            //pConnection->CloseConnection();
            m_conncectionPool.ReleaseConnection(pConnection);
        }
    }
    return usedCnt;
}
//
//TInt32 CConnectionMgr::Disconnect(TUInt32 connectionIdx)
//{
//    if (connectionIdx > m_maxConnectionNum)
//    {
//        return OUT_OF_RANGE;
//    }
//    switch (m_pConnectionPool[connectionIdx].GetConnectionState())
//    {
//        
//        case connection_is_not_in_use:
//        {
//            return CONNECTION_NOT_ESTABLISHED;
//        }
//        break;
//        case connection_is_trying:
//        {
//            m_pConnectionPool[connectionIdx].Disconnect();
//            return SUCCESS;
//        }
//        break;
//        case connection_is_established:
//        {
//            m_pConnectionPool[connectionIdx].Disconnect();
//            return SUCCESS;
//        }
//        break;
//        case connection_is_broken:
//        {
//            return SUCCESS;
//        }
//        break;
//        default:
//        {
//            return CONNECTION_IN_WRONG_STATE;
//        }
//    }
//}

TInt32 CConnectionMgr::Listen(const TChar *pIp,TUInt16 port,TUInt16 maxConnection,void *pIfCallBack)
{
    CListener *p = new CListener();
    if (NULL == p)
    {
        return OUT_OF_RANGE;
    }
    TUInt32 myIp = 0;
    if (pIp != NULL)
    {
        myIp = inet_addr(pIp);
    }
    TInt32 ret = p->Init(myIp, port, maxConnection,(IfListenerCallBack*)pIfCallBack,&m_conncectionPool,m_pParserFactory,m_pCryptorFactory);
    if (SUCCESS > ret)
    {
        p->Final();
        delete p;
        return ret;
    }
    p->AttachToList(m_pListeners);
    m_pListeners = p;
    return (TInt32)p;
}

TInt32 CConnectionMgr::StopListening(TInt32 listeningIdx)
{
    CListener *pListener = m_pListeners;
    CListener *pLast=NULL;
    while(pListener)
    {
        if (pListener == ((CListener *)listeningIdx))
        {
            if(pLast)
            {
                pLast->AttachToList(pListener->GetNext());
                delete pListener;
                return SUCCESS;
            }
            else
            {
                m_pListeners = pListener->GetNext();
                delete pListener;
                return SUCCESS;
            }
        }
        else
        {
            pLast = pListener;
            pListener = pListener->GetNext();
        }
    }
    return OUT_OF_RANGE;
}


}

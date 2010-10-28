#include "WinNetTester.h"
#include "Mmsystem.h"
CWinNetTester::CWinNetTester(IfTaskMgr *pTaskMgr)
{
    
    m_pNet = CreateNet(pTaskMgr,this,&m_parserFactory);
    if(!m_pNet)
    {
        return;
    }
    
}

CWinNetTester::~CWinNetTester()
{
    
}

TInt32 CWinNetTester::OnInit()
{
    m_lastRunTime = timeGetTime();
    return SUCCESS;
}

TInt32 CWinNetTester::Init(const char *pMyIp,const char *pRemoteIp,unsigned short myPort,unsigned short remotePort,int passiveConnectionNr,int connectionNr,int initSendMsg)
{
    Init4Event();
    m_ConnectionPool.Init(connectionNr);
    strcpy(m_myIp,pMyIp);
    strcpy(m_remoteIp,pRemoteIp);
    m_myPort = myPort;
    m_remotePort = remotePort;
    int ret = m_pNet->Listen(pMyIp,myPort,connectionNr,&m_ConnectionPool);
    if (SUCCESS > ret)
    {
        return ret;
    }
    for (int i=0;i<passiveConnectionNr;++i)
    {
        CAppConnection *pConnection = m_ConnectionPool.GetConnection();
        pConnection->SetPassvieSendNr(initSendMsg);
        bool tryAgain = true;
        while(tryAgain)
        {
            
            int ret = m_pNet->Connect(pRemoteIp,pMyIp,remotePort,0,pConnection);
            if (SUCCESS > ret)
            {
                Sleep(15);
            }
            else
            {
                tryAgain = false;
            }
        }
    }
    return SUCCESS;
}


TInt32 CWinNetTester::OnFianl()
{
    return SUCCESS;
}

TInt32 CWinNetTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int usedCnt = m_pNet->Run(runCnt);
    if (!usedCnt)
    {
        Wait4Event();
        //printf("no work to do!");
    }
    unsigned long timeNow = timeGetTime();
    if ((timeNow - m_lastRunTime) > 1000)
    {
        m_lastRunTime = timeNow;
        int PoolNr = m_ConnectionPool.GetConnectionNr();
        for (int i = 0;i<PoolNr;++i)
        {
            CAppConnection *pConn = m_ConnectionPool.GetConnectionByIdx(i);
            if (pConn)
            {
                
                int ret =  pConn->Run();
                if (SUCCESS < ret)
                {
                    usedCnt += ret;
                }
                
                if (-5 == ret)
                {
                    int ret = m_pNet->Connect(m_remoteIp,m_myIp,m_remotePort,0,pConn);
                }
                if (-4 == ret)
                {
                    //m_ConnectionPool.ReleaseConnection(pConn);
                }
            }
        }
    }
    return usedCnt;
}

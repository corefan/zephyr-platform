#include "WinNetTester.h"
#include "Mmsystem.h"
CWinNetTester::CWinNetTester(IfTaskMgr *pTaskMgr)
{
    m_ConnectionPool.Init(64);
    m_pNet = CreateNet(pTaskMgr,&m_parserFactory);
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

TInt32 CWinNetTester::OnFianl()
{
    return SUCCESS;
}

TInt32 CWinNetTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int usedCnt = m_pNet->Run(runCnt);
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
                usedCnt += pConn->Run();
            }
        }
    }
    return usedCnt;
}

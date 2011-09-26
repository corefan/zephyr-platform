#include "WinNetTester.h"
#include "Mmsystem.h"
#include "time.h"
#include "GatewayTestErrorCode.h"

CWinNetTester::CWinNetTester(IfTaskMgr *pTaskMgr,int maxConnectionNr)
{
    m_pNet = CreateNet(pTaskMgr,&m_parserFactory,NULL,maxConnectionNr);
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
    m_ConnectionPool.Init(connectionNr);
    strcpy(m_myIp,pMyIp);
    strcpy(m_remoteIp,pRemoteIp);
    m_myPort = myPort;
    m_remotePort = remotePort;
    int ret = (int)m_pNet->Listen(pMyIp,myPort,connectionNr,&m_ConnectionPool);
    if (NULL == ret)
    {
        return ret;
    }
    for (int i=0;i<passiveConnectionNr;++i)
    {
        CAppConnection *pConnection = m_ConnectionPool.GetConnection();
        
        bool tryAgain = true;
        while(tryAgain)
        {
            //�����и�bug�����һ��ʼ����û�ɹ�����ô����Ͳ�������.
            int ret = m_pNet->Connect(pRemoteIp,pMyIp,remotePort,0,pConnection);
            if (SUCCESS > ret)
            {
                m_pNet->Run(128);
            }
            else
            {
                tryAgain = false;
            }
        }
        //if (0 == (i %2)) //ÿ10��������Ϣ�ȴ�һ��
        {
            Sleep(15);
            m_pNet->Run(128);
        }
    }
    return SUCCESS;
}

TInt32 CWinNetTester::ConnectMe(CAppConnection *pConnection)
{
    return m_pNet->Connect(m_remoteIp,m_myIp,m_remotePort,0,pConnection);
}


TInt32 CWinNetTester::OnFianl()
{
    return SUCCESS;
}

TInt32 CWinNetTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int usedCnt = m_pNet->Run(runCnt);
    srand(time(NULL));
    m_ConnectionPool.UpdateClock();
    if ((m_ConnectionPool.GetClock()->GetTimeGap(m_lastRunTime)) > 500)
    {
        unsigned long timeNow = m_ConnectionPool.GetClock()->GetLocalTime();
        m_lastRunTime = timeNow;
        CList<CAppConnection> *pList = m_ConnectionPool.GetUsingList();
        CListNode<CAppConnection> *pNode = pList->header();
        while(pNode)
        {
            CListNode<CAppConnection> *pNext = pNode->GetNext();
            TInt32 nRet = pNode->Routine();
            if (RETRY_CONNECTING == nRet) //��Ҫ��������.
            {
                nRet = ConnectMe(pNode);
                if (nRet >= SUCCESS)
                {
                    pNode->OnTryConnecting();
                }
            }
            pNode = pNext;
        }
    }
    return usedCnt;
}

#include "CommTester.h"



TInt32 CCommTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int usedCnt = 0;
    for (int i=0;i<m_nrOfComm;++i)
    {
        CMessageHeader *pMsg = m_pComms[i]->GetMsg();
        while (pMsg)
        {
            int id = pMsg->GetMethodId();
            
            if (id != pMsg->GetBodyLength())
            {
                printf("Msg incorrect!");
            }
            else
            {
                TUChar *pD = pMsg->GetBody();
                for (unsigned int i = 0;i<id ;++i)
                {
                    TUChar c = i;
                    if (c != pD[i])
                    {
                        printf("Msg incorrect!");
                    }
                }
            }
            m_pComms[i]->ReturnMsgBuff(pMsg);
            ++usedCnt;
        }
    }
    return usedCnt;
}

int CCommTester::Init(IfCommunicatorMgr *pMgr,int nrOfComm)
{
    m_pCommMgr = pMgr;
    for (int i =0;i<nrOfComm;++i)
    {
        m_pComms[i] = pMgr->RegisterWorker(i);
        if (!m_pComms[i])
        {
            printf("Register worker failed!");
        }
    }
    return SUCCESS;
}

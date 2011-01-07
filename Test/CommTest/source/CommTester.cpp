#include "CommTester.h"
#include "../Config/include/SettingFile.h"


TInt32 CCommTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int usedCnt = 0;
    for (int i=0;i<runCnt;++i)
    {
        CMessageHeader *pMsg = m_pComms->GetMsg();
        while (pMsg)
        {
            int id = pMsg->GetMethodId();
            if (id != 400)
            {
                printf("Msg Len Incorrect!");
            }
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
            
            CMessageHeader *pRtn = m_pComms->PrepareMsg(pMsg->GetBodyLength(),pMsg->GetMethodId(),*pMsg->GetDestDoidByIdx(),pMsg->GetSrcDoid(),1);
            TUChar *pD = pRtn->GetBody();
            for (unsigned int i = 0;i<id ;++i)
            {
                pD[i] = i;
            }
            m_pComms->SendMsg(pRtn);
            m_pComms->ReturnMsgBuff(pMsg);
            ++usedCnt;
            if (usedCnt >= runCnt)
            {
                return usedCnt;
            }
            pMsg = m_pComms->GetMsg();
        }
    }
    return usedCnt;
}

void CCommTester::OnStartTestOne(int nInitMsgNr,int nInitMsgLen,int srvNr)
{
    m_nInitSendMgrNr = nInitMsgNr;
    m_nInitSendMgrLen  = nInitMsgLen;
    m_nSrvNr = srvNr;
    CDoid dest = m_tSvrDoid;
    for (int i = 0;i<nInitMsgNr;++i)
    {
        dest.m_srvId = (i % srvNr);
        CMessageHeader *pMsg = m_pComms->PrepareMsg(m_nInitSendMgrLen,m_nInitSendMgrLen,m_tSvrDoid,&dest,1);
        if (!pMsg)
        {
            ++m_nFailedTime;
            return;
        }
        TUChar *pBuff = pMsg->GetBody();
        for (int i = 0;i<nInitMsgLen;++i)
        {
            pBuff[i] = i;
        }
        m_pComms->SendMsg(pMsg);
    }
}

int CCommTester::Init(IfCommunicatorMgr *pMgr,CDoid *pSrvDoid)
{
    m_tSvrDoid = * pSrvDoid;
    m_pCommMgr = pMgr;
    m_pComms = pMgr->RegisterWorker(pSrvDoid->m_srvId);
    if (!m_pComms)
    {
        printf("Register worker failed!");
        return OUT_OF_MEM;
    }
    m_nSuccessTime = 0;
    m_nFailedTime  = 0;
    m_nSrvNr       = 0;
    m_nInitSendMgrNr = 0;
    m_nInitSendMgrLen = 0;

    return SUCCESS;
}

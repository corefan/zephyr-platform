#include "CommTester.h"
#include "../Config/include/SettingFile.h"

CDoid *CCommTester::sm_pDoids = NULL;

TInt32 CCommTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    if(m_nBeginTime)
    {
        unsigned long long timeNow = m_pComms->GetPlatfromTime();
        if (timeNow > m_nBeginTime)
        {
            if ((timeNow-m_nBeginTime) >  10000)
            {
                m_nBeginTime = 0;
                //发消息
               
            }
        }
    }
    CConnectionEvent event;
    
    int usedCnt = m_pComms->GetNetEvent(event);
    while (usedCnt>=0)
    {
        if (event.m_nEvent == en_connection_is_established_event)
        {
            for (int n=0;n<m_nNodeNr;++n)
            {
                for (int ip=0;ip<m_nIpNr;++ip)
                {
                    for (int i = 0;i<m_nSrvNr;++i)
                    {
                        int offset = n*m_nIpNr*m_nSrvNr+ip*m_nSrvNr+i;
                        //这就是目标啦 sm_pDoids[offset];
                        
                    }
                }
            }
        }
        else
        {
            //断链了
        }
        usedCnt = m_pComms->GetNetEvent(event);
    }
    usedCnt = 0;
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
            //是m_nSrvNr就群发
            if ((m_nMsgReced % m_nSrvNr) == 0)
            {
                CMessageHeader *pRtn = m_pComms->PrepareMsg(pMsg->GetBodyLength(),pMsg->GetMethodId(),*pMsg->GetDestDoidByIdx(),sm_pDoids,m_nSrvNr,false);
                TUChar *pD = pRtn->GetBody();
                for (unsigned int i = 0;i<id ;++i)
                {
                    pD[i] = i;
                }
                m_pComms->SendMsg(pRtn);
            }
            else
            {
                //不发
            }
            ++m_nMsgReced;

            ++usedCnt;
            if (usedCnt >= runCnt)
            {
                return usedCnt;
            }
            m_pComms->ReturnMsgBuff(pMsg);

            pMsg = m_pComms->GetMsg();
        }
    }
    return usedCnt;
}

void CCommTester::OnStartTestOne(int nInitMsgNr,int nInitMsgLen,int srvNr,int nIpNr,int nNodeNr)
{
    if (!sm_pDoids)
    {
        int n = srvNr;
        if (nIpNr>0)
        {
            srvNr *= nIpNr;
        }
        else
        {
            nIpNr = 1;
        }
        if (nNodeNr>0)
        {
            srvNr *= nNodeNr;
        }
        else
        {
            nNodeNr = 1;
        }
        sm_pDoids = new CDoid[srvNr];
        for (int n=0;n<nNodeNr;++n)
        {
            for (int ip=0;ip<nIpNr;++ip)
            {
                for (int i = 0;i<srvNr;++i)
                {
                    int offset = n*nIpNr*srvNr+ip*srvNr+i;
                    sm_pDoids[offset].m_nodeId = n;
                    sm_pDoids[offset].m_virtualIp = ip;
                    sm_pDoids[offset].m_srvId = i;
                    sm_pDoids[offset].m_objId = 0;
                }
            }
        }
        
    }
    m_nInitSendMgrNr = nInitMsgNr;
    m_nInitSendMgrLen  = nInitMsgLen;
    m_nSrvNr = srvNr;
    m_nIpNr = nIpNr;
    m_nNodeNr = nNodeNr;
    m_nBeginTime = m_pComms->GetPlatfromTime();
    if (0 == m_nBeginTime) //确保不为0
    {
        m_nBeginTime = 1;
    }
    //for (int i = 0;i<nInitMsgNr;++i)
    
}

void CCommTester::OnStartTestOne(int nInitMsgNr,int nInitMsgLen,int srvNr)
{
    if (!sm_pDoids)
    {
        sm_pDoids = new CDoid[srvNr];
        for (int i = 0;i<srvNr;++i)
        {
            sm_pDoids[i] = m_tSvrDoid;
            sm_pDoids[i].m_srvId = i;
        }
    }
    m_nInitSendMgrNr = nInitMsgNr;
    m_nInitSendMgrLen  = nInitMsgLen;
    m_nSrvNr = srvNr;
    CDoid dest = m_tSvrDoid;
    //for (int i = 0;i<nInitMsgNr;++i)
//     {
//         dest.m_srvId = ((dest.m_srvId + 1) % srvNr);
//         CMessageHeader *pMsg = m_pComms->PrepareMsg(m_nInitSendMgrLen,m_nInitSendMgrLen,m_tSvrDoid,&dest,1,false);
//         if (!pMsg)
//         {
//             ++m_nFailedTime;
//             return;
//         }
//         TUChar *pBuff = pMsg->GetBody();
//         for (int i = 0;i<nInitMsgLen;++i)
//         {
//             pBuff[i] = i;
//         }
//         m_pComms->SendMsg(pMsg);
//     }
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
    m_nMsgReced = 0;
    return SUCCESS;
}

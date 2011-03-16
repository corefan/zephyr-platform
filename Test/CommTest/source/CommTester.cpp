#include "CommTester.h"
#include "../Config/include/SettingFile.h"

CDoid *CCommTester::sm_pDoids = NULL;

TInt32 CCommTester::Run(const TInt32 threadId,const TInt32 runCnt)
{
    if(m_nBeginTime) //������һ��ʱ��
    {
        unsigned long long timeNow = m_pComms->GetPlatfromTime();
        if (timeNow > m_nBeginTime)
        {
            if ((timeNow-m_nBeginTime) >  10000)
            {
                m_nBeginTime = 0;
                //����Ϣ
               
            }
        }
    }
    CConnectionEvent event;
    
    int eventNr = m_pComms->GetNetEvent(event);
    int usedCnt = 0;
    while (eventNr>=0)
    {
        if (event.m_nEvent == en_connection_is_established_event)
        {
            m_bIsConnected = 1;
        }
        else
        {
            //������
            m_bIsConnected = 0;
        }
        ++usedCnt;
        eventNr = m_pComms->GetNetEvent(event);
    }
    if (!m_bIsConnected)
    {
        return usedCnt;
    }
    if (1 == m_bIsConnected) //�������ӣ�Ҫ����ʼ��Ϣ
    {
        m_nLastGetAllRespTime = m_pComms->GetLocalTime();
        SendAllMessage();
        ++m_bIsConnected;
    }
    
    for (int i=usedCnt;i<runCnt;++i)
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
            
            ++m_nMsgReced;

            ++usedCnt;
            
            m_pComms->ReturnMsgBuff(pMsg);
            if (usedCnt >= runCnt)
            {
                return usedCnt;
            }
            if (0==(m_nMsgReced%m_nDoidNr))
            {
                
                int gap = m_pComms->GetTimeGap(m_nLastGetAllRespTime);
                m_nLastGetAllRespTime = m_pComms->GetLocalTime();
                m_nTotalSendGap += gap;
                if (gap < m_nMinSendGap)
                {
                    m_nMinSendGap = gap;
                }
                if (gap > m_nMaxSendGap)
                {
                    m_nMaxSendGap = gap;
                }
                ++m_nSendTime ;
                if (0 == (m_nSendTime%10000))
                {
                    printf("MinSendGap:%u,MaxSendGap:%u,totalSendGap:%u,sendTime:%u,averageSendGap:%u \n\t",m_nMinSendGap,m_nMaxSendGap,m_nTotalSendGap,m_nSendTime,(m_nTotalSendGap/m_nSendTime));
                }
                SendAllMessage();
            }
            pMsg = m_pComms->GetMsg();
        }
        //��m_nSrvNr��Ⱥ��
    }
    CheckAll();
    return usedCnt;
}

void CCommTester::OnStartTestOne(int nInitMsgNr,int nInitMsgLen,int srvNr,int nIpNr,int nNodeNr)
{
    
    {
        int totalNr = srvNr;
        if (nIpNr>0)
        {
            totalNr *= nIpNr;
        }
        else
        {
            nIpNr = 1;
        }
        if (nNodeNr>0)
        {
            totalNr *= nNodeNr;
        }
        else
        {
            nNodeNr = 1;
        }
        if (!sm_pDoids)
        {
            sm_pDoids = new CDoid[totalNr];
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
        m_nDoidNr = totalNr;
    }
   
    m_nInitSendMgrNr = nInitMsgNr;
    m_nInitSendMgrLen  = nInitMsgLen;
    m_nSrvNr = srvNr;
    m_nIpNr = nIpNr;
    m_nNodeNr = nNodeNr;
    m_nBeginTime = m_pComms->GetPlatfromTime();
    if (0 == m_nBeginTime) //ȷ����Ϊ0
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
    m_nLastSendTime = 0;
    m_bIsConnected = 0;
    m_nLastSendTime = m_pComms->GetLocalTime();
    m_nLastGetAllRespTime = 0;
    m_nMinSendGap = 10000000;
    m_nMaxSendGap = 0;
    m_nSendTime = 0;
    m_nTotalSendGap = 0;

    return SUCCESS;
}


void CCommTester::SendAllMessage()
{
    CMessageHeader *pMsg = m_pComms->PrepareMsg(m_nInitSendMgrLen,m_nInitSendMgrLen,m_tSvrDoid,sm_pDoids,m_nDoidNr,true);
    if (pMsg)
    {
        unsigned char *pBuff = (unsigned char*)pMsg->GetBody();
        for (unsigned int i=0;i<m_nInitSendMgrLen;++i)
        {
            pBuff[i] = (unsigned char)i;
        }
        m_pComms->SendMsg(pMsg);
        m_nLastSendTime = m_pComms->GetLocalTime();
    }
}

void CCommTester::CheckAll()
{
    if (m_pComms->GetTimeGap(m_nLastSendTime) > 5000)
    {
        m_nLastSendTime = m_pComms->GetLocalTime();
        SendAllMessage();
    }
}
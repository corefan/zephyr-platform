#include "CommTester.h"
#include "../Config/include/SettingFile.h"


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
    int initSendMsgNr = 0;
    int initSendMsgLen = 0;
    CDoid initSendMsgDoid;
    CSettingFile setting;
    if (setting.LoadFromFile("CommTestConfig.ini"))
    {
        initSendMsgNr = setting.GetInteger("MAIN","initSendMsgNr",0);
        initSendMsgLen = setting.GetInteger("MAIN","initSendMsgLen",0);
        initSendMsgDoid.m_nodeId = setting.GetInteger("DOID","nodeId",0);
        initSendMsgDoid.m_virtualIp = setting.GetInteger("DOID","m_virtualIp",0);
        initSendMsgDoid.m_srvId    = setting.GetInteger("DOID","m_srvId",0);
        initSendMsgDoid.m_objId  = setting.GetInteger("DOID","m_objId",0);
    }
    else
    {
        printf("Can not open file CommTestConfig.ini");
        return FAIL;
    }
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

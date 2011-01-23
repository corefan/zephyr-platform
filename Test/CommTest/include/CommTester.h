#ifndef __ZEPHYR_TEST_XXX_TESTERH__
#define __ZEPHYR_TEST_XXX_TESTERH__
#include "TypeDef.h"
#include "SysInc.h"
//Task
#include "TaskCenter.h"
#include "SysMacros.h"
#include "IfLoggerMgr.h"
#include "ExceptionParser.h"
#include "IfCommunicator.h"

using namespace Zephyr;

//每秒
//可以测试同Ip间对发，测试同Node间的对法，测试不同node间的路由。
//测试同Ip间的多发，测试同Node间的多发，测试不同node间的路由
//2011-01-05,完成同Ip见的对法，不走ip转.
class CCommTester : public IfTask
{
protected:
    IfCommunicatorMgr *m_pCommMgr;
    IfCommunicator    *m_pComms;
    CDoid              m_tSvrDoid;
    int                m_nInitSendMgrNr;
    int                m_nInitSendMgrLen;
    int                m_nSrvNr;
    short              m_nIpNr;
    short              m_nNodeNr;
    unsigned int       m_nSuccessTime;
    unsigned int       m_nFailedTime;
    unsigned int       m_nMsgReced;
    static CDoid       *sm_pDoids;
    unsigned long long        m_nBeginTime;
public:
    int Init(IfCommunicatorMgr *pMgr,CDoid *pSrvDoid);
    void OnStartTestOne(int nInitMsgNr,int nInitMsgLen,int srvNr);
    void OnStartTestOne(int nInitMsgNr,int nInitMsgLen,int srvNr,int nIpNr,int nNodeNr);
    virtual TInt32 Begin(TInt32 threadId)
    {
        return SUCCESS;
    }
    virtual TInt32 Run(const TInt32 threadId,const TInt32 runCnt);
    virtual TInt32 End(TInt32 threadId)
    {
        return SUCCESS;
    }
};

#endif

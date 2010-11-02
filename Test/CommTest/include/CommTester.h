#ifndef __ZEPHYR_TEST_XXX_TESTERH__
#define __ZEPHYR_TEST_XXX_TESTERH__
#include "TypeDef.h"
#include "SysInc.h"
//Task
#include "TaskCenter.h"
#include "SysMacros.h"
#include "IfLoggerMgr.h"
#include "ExceptionParser.h"
using namespace Zephyr;

class CCommTester : public IfTask
{
protected:
    IfCommunicatorMgr *m_pCommMgr;
    IfCommunicator    *m_pComms[24];
public:
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

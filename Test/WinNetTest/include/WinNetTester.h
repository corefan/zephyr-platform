#ifndef __ZEPHYR_TEST_XXX_TESTERH__
#define __ZEPHYR_TEST_XXX_TESTERH__
#include "TypeDef.h"
#include "SysInc.h"
//Task
#include "TaskCenter.h"
#include "SysMacros.h"
#include "IfLoggerMgr.h"
#include "ExceptionParser.h"
#include "NetCenter.h"
#include "AppConnectionMgr.h"
#include "ParserFactory.h"

using namespace Zephyr;
class CWinNetTester : public IfTask
{
public:
    IfNet*              m_pNet;
    CAppConnectionMgr  m_ConnectionPool;
    CTestParserFactory  m_parserFactory;
    unsigned long       m_lastRunTime;
public:
    CWinNetTester(IfTaskMgr *pTaskMgr);
    ~CWinNetTester();
    TInt32 OnInit();
    TInt32 OnFianl();
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

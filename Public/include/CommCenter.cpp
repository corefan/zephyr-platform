#include "CommCenter.h"
#include "SysMacros.h"
#include "TypeDef.h"
#include "..\Comm\include\CommMgr.h"

namespace Zephyr
{
IfCommunicatorMgr *CreateCommMgr(TUInt32 nrOfAppWorkerNeeded,IfTaskMgr *pTaskMgr,IfLoggerManager *pLoggerMgr,TChar *pConfigName)
{
    CCommMgr *pCommMgr = new CCommMgr();
    pCommMgr->Init(pTaskMgr,pLoggerMgr,pConfigName);
    return pCommMgr;
}
}

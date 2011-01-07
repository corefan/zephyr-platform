#include "CommCenter.h"
#include "SysMacros.h"
#include "TypeDef.h"
#include "..\Comm\include\CommMgr.h"

namespace Zephyr
{
IfCommunicatorMgr *CreateCommMgr(int nrOfWorkerThread,IfTaskMgr *pTaskMgr,IfLoggerManager *pLoggerMgr,TChar *pConfigName)
{
    CCommMgr *pCommMgr = new CCommMgr();
    pCommMgr->Init(nrOfWorkerThread,pTaskMgr,pLoggerMgr,pConfigName);
    return pCommMgr;
}
}

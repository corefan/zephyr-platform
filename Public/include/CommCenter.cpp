#include "CommCenter.h"
#include "SysMacros.h"
#include "TypeDef.h"
#include "..\Comm\include\CommMgr.h"

namespace Zephyr
{
IfCommunicatorMgr *CreateCommMgr(TInt32 nrOfWorkerThread,IfTaskMgr *pTaskMgr,IfLoggerManager *pLoggerMgr,const TChar *pConfigName)
{
    CCommMgr *pCommMgr = new CCommMgr();
    pCommMgr->Init(nrOfWorkerThread,pTaskMgr,pLoggerMgr,pConfigName);
    return pCommMgr;
}

IfCommunicatorMgr *CreateCommMgrWithConfig(TInt32 nrOfWorkerThread,IfTaskMgr *pTaskMgr,IfLoggerManager *pLoggerMgr,const TChar *pConfigStr)
{
    CCommMgr *pCommMgr = new CCommMgr();
    pCommMgr->InitWithConfig(nrOfWorkerThread,pTaskMgr,pLoggerMgr,pConfigStr);
    return pCommMgr;
}

void ReleaseCommMgr(IfCommunicatorMgr *pIfComm)
{
	CCommMgr *pCommMgr = dynamic_cast<CCommMgr*>(pIfComm);
	if (pCommMgr)
	{
        pCommMgr->OnFinal();
		delete pCommMgr;
	}
}

void GiveCommMoreCpu(IfCommunicatorMgr *pIfComm,IfTaskMgr *pTaskMgr)
{
	CCommMgr *pCommMgr = dynamic_cast<CCommMgr*>(pIfComm);
	if (pCommMgr)
	{
		pCommMgr->GiveMoreCpu(pTaskMgr);
	}
}

}

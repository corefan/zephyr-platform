// ServiceContainer.cpp : 定义控制台应用程序的入口点。
//
//这个是用来加载DLLservice的，需要提供findService

#include "../Public/include/SysMacros.h"
#include "../Public/include/TypeDef.h"
#include "include/Orb.h"
#include "../Public/include/LogCenter.h"
#include "../Public/include/CommCenter.h"
#include "../Public/include/TaskCenter.h"
#include "../System/ExceptionParser/include/ExceptionParser.h"
#include "include/ServiceContainerCfg.h"
using namespace Zephyr;
int main(int argc, char* argv[])
{
    //先读配置
    CServiceCfgRead tRead;
    
    if (argc)
    {
        if (tRead.Read(argv[0]) < SUCCESS)
        {
            printf("Read ServiceContainer Config:%s failed",argv[0]);
            return FAIL;
        }
    }
    else
    {
        if (tRead.Read("DefaultConfig.ini") < SUCCESS)
        {
            printf("Read Default ServiceContainer Config failed");;
            return FAIL;
        }
    }

    //创建TaskMgr
    IfTaskMgr *pTaskMgr = CreateTaskMgr();


    CExceptionParser parser;

    //创建LoggerMgr
    IfLoggerManager *pLogMgr = CreateLogSys(pTaskMgr);
    //创建Comm
    IfCommunicatorMgr *pMgr = CreateCommMgr(tRead.m_tCfg.m_nNrOfOrb,pTaskMgr,pLogMgr,&tRead.m_tCfg.m_pszCommConfigName);
    //创建Orb
    COrb *pOrb = new COrb[tRead.m_tCfg.m_nNrOfOrb];
    for (int i=0;i<tRead.m_tCfg.m_nNrOfOrb;++i)
    {
        TUInt16 nSrvBegin,nSrvEnd;
        IfCommunicator *pIfComm = pMgr->GetComm(nSrvBegin,nSrvEnd,i);
        if (pIfComm)
        {
            CDoid doid;
            doid.m_objId = 0;
            doid.m_srvId = nSrvBegin;
            doid.m_virtualIp = tRead.m_tCfg.m_nLocalIp;
            doid.m_nodeId    = tRead.m_tCfg.m_nLocalNodeId;
            pOrb[i].Init(pIfComm,&doid,tRead.m_tCfg.m_pOrbs[i].m_nStubNr);
        }
    }
    //加载Service

    //完了
	return 0;
}


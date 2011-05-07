#ifndef __ZEPHYR_SERVICE_CONTAINER_CFG_H__
#define __ZEPHYR_SERVICE_CONTAINER_CFG_H__

#include "../../Public/include/TypeDef.h"
#include "../../Public/include/SysMacros.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace Zephyr
{

class IfOrb;
class IfTaskMgr;
class IfLoggerManager;
class CService;

#define SERVICE_INIT_FUN_NAME	"InitService"
    typedef CService* (*SERVICE_INIT_FUN)(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr);

#define SERVICE_RELEASE_FUN_NAME "ReleaseService"
    typedef int (*SERVICE_RELEASE_FUN)();



struct TServiceConfig
{
    //这个Service的dll
    const TChar *m_pszServiceDllName;
#ifdef _WIN32
    HMODULE				m_pPluginModuleHandle;
#else
    void*               m_pPluginModuleHandle
#endif
    SERVICE_INIT_FUN	    m_pInitFun;
    SERVICE_RELEASE_FUN	    m_pReleaseFun;
};


//一个Service可以有多个
struct TOrbConfig
{
    TOrbConfig()
    {
        m_nNrofService = 0;
        m_pServices = NULL;
    }
    ~TOrbConfig()
    {
        if (m_pServices)
        {
            delete [] m_pServices;
            m_pServices = NULL;
        }
    }
    TInt32 m_nNrofService;
    TUInt32 m_nStubNr;
    TServiceConfig *m_pServices; 
};

struct TServiceContainerCfg
{
    TServiceContainerCfg()
    {
        m_nNrOfOrb = 0;
        m_pOrbs = NULL;
        m_pszCommConfigName = NULL;
    }
    ~TServiceContainerCfg()
    {
        if (m_pOrbs)
        {
            delete [] m_pOrbs;
        }
    }
    TInt32 m_nNrOfOrb;
    TInt16  m_nLocalIp;
    TInt16  m_nLocalNodeId;
    const TChar *m_pszCommConfigName;
    TOrbConfig *m_pOrbs;
    TInt16  m_nWorkerNr;
    TInt16  m_nCpuNr;
};

class CServiceCfgRead
{
public:
    TServiceContainerCfg m_tCfg;
    //从这儿读取.
    TInt32 Read(TChar *pFileName);

};


}
#endif
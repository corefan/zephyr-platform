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
#include "../System/Logger/include/LoggerManager.h"
#include "./include/Service.h"
using namespace Zephyr;



#ifdef WIN32

HMODULE LoadDynamicLib(const char * LibFileName)
{
    return LoadLibrary(LibFileName);
}

void ReleaseDynamicLib(HMODULE LibHandle)
{
    FreeLibrary(LibHandle);
}

void * GetDynamicLibFunction(HMODULE LibHandle,char * FunctionName)
{
    return GetProcAddress(LibHandle,FunctionName);
}

char * dlerror()
{
    static char ErrBuff[20];
    sprintf(ErrBuff,"%d",GetLastError());
    return ErrBuff;
}

#else


void * LoadDynamicLib(const char * LibFileName)
{
    return dlopen(LibFileName,RTLD_NOW);
}

void ReleaseDynamicLib(void * LibHandle)
{
    dlclose(LibHandle);
}

void * GetDynamicLibFunction(void * LibHandle,char * FunctionName)
{
    return dlsym(LibHandle,FunctionName);
}

#endif




int main(int argc, char* argv[])
{
    //先读配置
    CServiceCfgRead tRead;
    
    if (argc>1)
    {
        if (tRead.Read(argv[1]) < SUCCESS)
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
    if (!pTaskMgr)
    {
        printf("Create TaskMgr failed!");
        return OUT_OF_MEM;
    }

    CExceptionParser parser;

    //创建LoggerMgr
    IfLoggerManager *pLogMgr = CreateLogSys(pTaskMgr);
    if (!pLogMgr)
    {
        printf("Create LogMgr failed!");
        return OUT_OF_MEM;
    }
    //创建Comm
    IfCommunicatorMgr *pMgr = CreateCommMgr(tRead.m_tCfg.m_nNrOfOrb,pTaskMgr,pLogMgr,tRead.m_tCfg.m_pszCommConfigName);
    if (!pMgr)
    {
        printf("Create CommMgr failed!");
        return OUT_OF_MEM;
    }
    //创建Orb
    COrb *pOrb = NULL;
    try 
    {
        pOrb = new COrb[tRead.m_tCfg.m_nNrOfOrb];
    }
    catch(...)
    {
        printf("Create Orb[%d] failed!",tRead.m_tCfg.m_nNrOfOrb);
        return OUT_OF_MEM;
    }
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
            TInt32 nRet = pOrb[i].Init(pIfComm,&doid,tRead.m_tCfg.m_pOrbs[i].m_nSkeleton);
            if (nRet < SUCCESS)
            {
                printf("Orb[%d] init failed! stub nr:%d",i,tRead.m_tCfg.m_pOrbs[i].m_nSkeleton);
                return nRet;
            }
        }
        else
        {
            printf("Get IfComm[%d] failed!",i);
            return NULL_POINTER;
        }
    }
    //加载ServiceMgr,这个再写好了


    //加载Service
    for (int i=0;i<tRead.m_tCfg.m_nNrOfOrb;++i)
    {
        for (int j=0;j<tRead.m_tCfg.m_pOrbs[i].m_nNrofService;++j)
        {
            tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle = LoadDynamicLib(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pszServiceDllName.c_str());
            if (tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle)
            {

                tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pInitFun = (SERVICE_INIT_FUN)GetDynamicLibFunction(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle,SERVICE_INIT_FUN_NAME);
                tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pReleaseFun=(SERVICE_RELEASE_FUN)GetDynamicLibFunction(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle,SERVICE_RELEASE_FUN_NAME);

                printf("Plugin [%s] InitFun=%p ReleaseFun=%p",
                    tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pszServiceDllName,
                    tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pInitFun,
                    tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pReleaseFun);

                if(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pInitFun&&tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pReleaseFun)
                {   
                    TInt32 nRegisterSuccess = -1;
                    CService *pService = (*(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pInitFun))(pOrb+i,pTaskMgr,pLogMgr);
                    if(pService)
                    {
                        IfSkeleton *pSkeleton = pOrb[i].RegiterService(pService,pService->GetServiceId());
                        if (pSkeleton)
                        {
                            pService->SetSkeleton(pSkeleton);
                            printf("Init Plugin [%s] Succeed",tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pszServiceDllName);
                            nRegisterSuccess = pTaskMgr->AddTask(pOrb+i);
                        }
                    }
                    
                    if (nRegisterSuccess<SUCCESS)
                    {
                        printf("Init Plugin [%s] Failed",tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pszServiceDllName);
                        ReleaseDynamicLib(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle);
                        tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle = NULL;
                    }
                }
                else
                {
                    printf("Invalid Plugin [%s]",tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pszServiceDllName);
                    ReleaseDynamicLib(tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle);
                    tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pPluginModuleHandle = NULL;
                }


            }
            else
            {
                printf("Load DLL:%s failed!",tRead.m_tCfg.m_pOrbs[i].m_pServices->m_pszServiceDllName);
            }
        }
    }
    pTaskMgr->AddTask((CLoggerManager*)pLogMgr);
    pTaskMgr->StartWorking(tRead.m_tCfg.m_nWorkerNr,tRead.m_tCfg.m_nCpuNr);  
    //完了
	return 0;
}


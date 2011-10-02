#include "../include/AuthenticateService.h"
#include "../include/DBConfig.h"
#include "../../Interface/include/IfAuthServiceSkeleton.h"

namespace Zephyr
{

IMPLEMENT_START_HANDLE_INTERFACE(CAuthenticateService)
IMPLEMENT_HANDLE_INTERFACE(IfAuthService)
IMPLEMENT_END_HANDLE_INTERFACE(CAuthenticateService)


CAuthenticateService::CAuthenticateService()
{
    m_pDbMgr        = NULL;
    m_pIfOrb        = NULL;
    m_pIfTaskMgr    = NULL;
    m_pLoggerMgr    = NULL;
    m_pLogger       = NULL;
}

CAuthenticateService::~CAuthenticateService()
{
    if (m_pDbMgr)
    {
        ReleaseMgr(m_pDbMgr);
        m_pDbMgr = NULL;
    }
    if (m_pLogger)
    {
        m_pLoggerMgr->ReleaseLogger(m_pLogger);
        m_pLogger = NULL;
    }
}

TInt32 CAuthenticateService::Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData)
{
    
    return SUCCESS;
}




CService *InitService(IfOrb* pOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr)
{
    CAuthenticateService *pAuthenticateScrvice=NULL;
    try
    {
        pAuthenticateScrvice = new CAuthenticateService();
    }
    catch (...)
    {
        return NULL;
    }
    pAuthenticateScrvice->InitService(pOrb,pIfTaskMgr,pIfLoggerMgr);
    return pAuthenticateScrvice;
}

TInt32 ReleaseService(CService* pService)
{
    CAuthenticateService *pAS = (CAuthenticateService*)pService;
    delete pAS;
    return SUCCESS;
}

//在初始化的时候会被调.
TInt32 CAuthenticateService::OnInit()
{
    //m_pDbMgr = GetMysqlWorkThreadMgr()
    CDBConfig tDBConfig;
    if (tDBConfig.ReadConfig("DBConfig.ini") < 0)
    {
        return FAIL;
    }
    TInt32 nLoggerIdx = m_pLoggerMgr->AddLogger(tDBConfig.m_szLoggerName,-1);
    if (nLoggerIdx < SUCCESS)
    {
        return nLoggerIdx;
    }
    IfLogger *pDBLogger = m_pLoggerMgr->GetLogger(nLoggerIdx);
    if (!pDBLogger)
    {
        return NULL_POINTER;
    }
    m_pDbMgr = GetMysqlWorkThreadMgr(tDBConfig.m_szConnectStr,pDBLogger,tDBConfig.m_nThreadCount,tDBConfig.m_nQueueSize,tDBConfig.m_nFlag);
    if (!m_pDbMgr)
    {
        printf("Init DB failed, Pls check the DB config:DBConfig.ini");
        return FAIL;
    }
    m_nMaxTransNum = tDBConfig.m_nThreadCount * tDBConfig.m_nQueueSize;
    TInt32 nRet = m_tTransPool.InitPool(m_nMaxTransNum);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    m_tUsingMaps.Init(&m_tTransPool);

    return SUCCESS;
}

TInt32 CAuthenticateService::OnDisconneted(CDoid tMyDoid)
{
    TplNode<CDBAuthenticateTrans,CDoid> *pNode = m_tUsingMaps.GetItemByKey(tMyDoid);
    if (pNode)
    {
        pNode->OnDisconnected();
    }
    return SUCCESS;
}

TInt32 CAuthenticateService::InitService(IfOrb *pIfOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pLoggerMgr)
{
    m_pIfOrb = pIfOrb;
    m_pIfTaskMgr = pIfTaskMgr;
    m_pLoggerMgr = pLoggerMgr;
    return SUCCESS;
}

//结束是回调.
TInt32 CAuthenticateService::OnFinal()
{
    m_tUsingMaps.UnInit();
    return SUCCESS;
}


}
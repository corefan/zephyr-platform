#include "../include/AuthenticateService.h"
#include "../include/DBConfig.h"
#include "../../Interface/include/IfAuthServiceSkeleton.h"
#include "../include/AuthenticateLogger.h"
#include "../../Interface/include/IfAuthRespStub.h"


#pragma  comment(lib,"libmysql.lib")

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
    m_nMaxTransNum  = 0;
    m_nPendingDBTrans = 0;
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

    m_pIfOrb        = NULL;
    m_pIfTaskMgr    = NULL;
    m_pLoggerMgr    = NULL;
    m_nMaxTransNum  = 0;
    m_nPendingDBTrans = 0;
}

TInt32 CAuthenticateService::Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData)
{
    CDoid *pFrom = GetMyDoid();
    IfAuthResp *pResp;
    GET_REMOTE_STUB_PT(pResp,IfAuthResp,pFrom);
    if (pFrom)
    {
        //之前已经在请求登陆了，不必
        CDBAuthenticateTrans *pDB = m_tUsingMaps.GetItemByKey(*pFrom);
        if (pDB)
        {
            pResp->RespAuthenticate(-((TInt32)en_reading_db),tAuthenticateData);
            return SUCCESS;
        }
        if (m_nPendingDBTrans < m_nMaxTransNum)
        {
            if (tAuthenticateData.GetBodyLength() == sizeof(CAuthenticateData))
            {
                pDB = m_tUsingMaps.PrepareItem();
                if (pDB) //分配内存成功
                {
                    ++m_nPendingDBTrans;
                    ++m_nTotalReqTrans;
                    ++m_nTotalReqTransIn1Min;
                    pDB->Init((CAuthenticateData*)tAuthenticateData.GetBody(),this);
                    pDB->m_tSrcDoid = *pFrom;
                    IfTrasactionWorkThread *pThread = m_pDbMgr->GetThread();
                    if(pThread)
                    {
                        if (pThread->AddTransaction(pDB))
                        {
                            //必须成功啊
                            m_tUsingMaps.AddInTree(pDB);
                        }
                        else
                        {
                            m_tUsingMaps.ReleaseItem(pDB);
                        }
                        return SUCCESS;
                    }
                }
            }
        }
        //else
        {
           
            //把原数据发回
            pResp->RespAuthenticate(-((TInt32)en_system_is_too_busy),tAuthenticateData);
            //写日志
            LOG_RUN(en_system_is_too_busy,"System is to busy!");
        }
    }
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
TInt32 CAuthenticateService::OnInited()
{
    //m_pDbMgr = GetMysqlWorkThreadMgr()
    CDBConfig tDBConfig; 
    TChar szPathFile[128];
    sprintf(szPathFile,"%d/DBConfig.ini",GetMyDoid()->m_virtualIp);
    if (tDBConfig.ReadConfig(szPathFile) < 0)
    {
        return FAIL;
    }
    return StartService(tDBConfig.m_szLoggerName,tDBConfig.m_szConnectStr,tDBConfig.m_nThreadCount,tDBConfig.m_nQueueSize,tDBConfig.m_nFlag);
}

TInt32 CAuthenticateService::StartService(TChar *pszLoggerName,TChar *pszConnectStr,TInt32  nThreadCount,TInt32 nQueueSize,TUInt32 uFlag)
{
    TChar szDBLoggerName[200];
    if (NULL == pszLoggerName)
    {
        return NULL_POINTER;
    }
    if (0 == pszLoggerName[0])
    {
        pszConnectStr = "DBLogger.Log";
    }
    sprintf(szDBLoggerName,"DBLib_%s",pszLoggerName);
    TInt32 nLoggerIdx = m_pLoggerMgr->AddLogger(szDBLoggerName,-1);
    if (nLoggerIdx < SUCCESS)
    {
        return nLoggerIdx;
    }
    IfLogger *pDBLogger = m_pLoggerMgr->GetLogger(nLoggerIdx);
    if (!pDBLogger)
    {
        return NULL_POINTER;
    }
    m_pDbMgr = GetMysqlWorkThreadMgr(pszConnectStr,pDBLogger,nThreadCount,nQueueSize,uFlag);
    if (!m_pDbMgr)
    {
        printf("Init DB failed, Pls check the DB config:DBConfig.ini");
        return FAIL;
    }
    m_nMaxTransNum = nThreadCount * nQueueSize;
    TInt32 nRet = m_tTransPool.InitPool(m_nMaxTransNum);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    m_tUsingMaps.Init(&m_tTransPool);
    nLoggerIdx =  m_pLoggerMgr->AddLogger(pszLoggerName,-1);
    if (nLoggerIdx < SUCCESS)
    {
        return nLoggerIdx;
    }
    m_pLogger = m_pLoggerMgr->GetLogger(nLoggerIdx);
    //其實只能是40ms
    return m_pIfOrb->RegisterRun(GetSkeleton(),40);
}



TInt32 CAuthenticateService::OnDisconneted(CDoid tMyDoid)
{
    TplNode<CDBAuthenticateTrans,CDoid> *pNode = m_tUsingMaps.GetItemByKey(tMyDoid);
    if (pNode)
    {
        TChar szDoid[64];
        tMyDoid.ToStr(szDoid);
        LOG_RUN(en_client_disconneted,"Client:%s Disconneted before DB return!\n",szDoid);
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
void CAuthenticateService::OnFinal()
{
    m_tUsingMaps.UnInit();
}

void CAuthenticateService::OnDbFinished(CDBAuthenticateTrans *pTrans)
{
    --m_nPendingDBTrans;
    ++m_nTotalRetTrans;
    CDoid *pFrom = &pTrans->m_tSrcDoid;
    if (pTrans->IsContinue())
    {
        if (pTrans->IsSuccess())
        {
            //回覆成功
            IfAuthResp *pResp;
            GET_REMOTE_STUB_PT(pResp,IfAuthResp,pFrom);
            //把原数据发回
            TLV<TUInt16,TUInt16> tAuthTLV;
            tAuthTLV.m_nBodyLength = sizeof(CAuthorityData);
            tAuthTLV.m_nTag = pTrans->m_nResult; //表示成功.其实没用
            tAuthTLV.m_pBuffer = (TUChar*)&pTrans->m_unAllData.m_tAuthorityData;
            pResp->RespAuthenticate(-((TInt32)en_incorrect_data_length),tAuthTLV);
            //成功后，要为其创建后续的Session..
        }
        else
        {
            //回覆失敗
            IfAuthResp *pResp;
            GET_REMOTE_STUB_PT(pResp,IfAuthResp,pFrom);
            //把原数据发回
            TLV<TUInt16,TUInt16> tAuthTLV;
            tAuthTLV.m_nBodyLength = sizeof(CAuthenticateData);
            tAuthTLV.m_nTag = 0; //表示元数据返回
            tAuthTLV.m_pBuffer = (TUChar*)&pTrans->m_unAllData.m_tAuthenticateData;
            pResp->RespAuthenticate(-((TInt32)en_data_trans_failed),tAuthTLV);
            TChar szDoid[64];
            pFrom->ToStr(szDoid);
            LOG_RUN(en_data_trans_failed,"DB Trans Failed for Doid:%s\n",szDoid);
        }
    }
    //從樹中刪除
    m_tUsingMaps.RemoveFromTree(pTrans);
    //放回內存池
    m_tUsingMaps.ReleaseItem(pTrans);
}

TInt32  CAuthenticateService::OnRoutine(TUInt32 nRunCnt)
{
    TInt32 uUsed = CService::OnRoutine(nRunCnt);
    if (uUsed > nRunCnt)
    {
        return uUsed;
    }
    uUsed += m_pDbMgr->Update(nRunCnt);
    TInt32 nGap = m_pClock->GetTimeGap(m_nLastStaticTime);
    if (nGap > 60)
    {
        m_nLastStaticTime = m_pClock->GetLocalTime();
        LOG_RUN(en_statistic_db_trans_num,"Total Trans %llu, Total Return %llu, %u in last minutes.\n",m_nTotalReqTrans,m_nTotalRetTrans,m_nTotalReqTransIn1Min);
        m_nTotalReqTransIn1Min = 0;
    }
    return uUsed;
}

}
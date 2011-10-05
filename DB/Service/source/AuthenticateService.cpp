#include "../include/AuthenticateService.h"
#include "../include/DBConfig.h"
#include "../../Interface/include/IfAuthServiceSkeleton.h"
#include "../include/AuthenticateLogger.h"
#include "../../Interface/include/IfAuthRespStub.h"

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
    if (pFrom)
    {
        CDBAuthenticateTrans *pDB = m_tUsingMaps.GetItemByKey(*pFrom);
        if (pDB)
        {
            pDB->OnDisconnected();
            m_tUsingMaps.ReleaseItem(pDB);
            //等数据库返回后在释放回内存池
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
                    }
                }
            }
        }
        else
        {
            IfAuthResp *pResp;
            GET_REMOTE_STUB_PT(pResp,IfAuthResp,pFrom);
            //把原数据发回
            pResp->RespAuthenticate(-((TInt32)en_incorrect_data_length),tAuthenticateData);
            //写日志
            TChar szDoid[64];
            pFrom->ToStr(szDoid);
            LOG_RUN(en_incorrect_data_length,"Recive incorrect data from Doid:%s",szDoid);
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
TInt32 CAuthenticateService::OnInit()
{
    //m_pDbMgr = GetMysqlWorkThreadMgr()
    CDBConfig tDBConfig; 
    if (tDBConfig.ReadConfig("DBConfig.ini") < 0)
    {
        return FAIL;
    }
    return StartService(tDBConfig.m_szLoggerName,tDBConfig.m_szConnectStr,tDBConfig.m_nThreadCount,tDBConfig.m_nQueueSize,tDBConfig.m_nFlag);
}

TInt32 CAuthenticateService::StartService(TChar *pszLoggerName,TChar *pszConnectStr,TInt32  nThreadCount,TInt32 nQueueSize,TUInt32 uFlag)
{
    TInt32 nLoggerIdx = m_pLoggerMgr->AddLogger(pszLoggerName,-1);
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
        LOG_RUN(en_client_disconneted,"Client:%s Disconneted before DB return!",szDoid);
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

void CAuthenticateService::OnDbFinished(CDBAuthenticateTrans *pTrans)
{
    --m_nPendingDBTrans;
    ++m_nTotalRetTrans;
    if (pTrans->IsContinue())
    {
        if (pTrans->IsSuccess())
        {
            //回覆成功
        }
        else
        {
            //回覆失敗
        }
    }
    //從樹中刪除
    m_tUsingMaps.RemoveFromTree(pTrans);
    //放回內存池
    m_tUsingMaps.ReleaseItem(pTrans);
}

TInt32  CAuthenticateService::OnRoutine(TUInt32 nRunCnt)
{
    TInt32 nGap = m_pClock->GetTimeGap(m_nLastStaticTime);
    if (nGap > 60)
    {
        m_nLastStaticTime = m_pClock->GetLocalTime();
        LOG_RUN(en_statistic_db_trans_num,"Total Trans %llu, Total Return %llu, %u in last minutes.\n",m_nTotalReqTrans,m_nTotalRetTrans,m_nTotalReqTransIn1Min);
        m_nTotalReqTransIn1Min = 0;
    }
    return SUCCESS;
}

}
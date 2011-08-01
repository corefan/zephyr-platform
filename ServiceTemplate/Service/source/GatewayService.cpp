#include "../include/GatewayService.h"
#include "Public/include/SysMacros.h"
#include "../../Interface/include/IfGatewaySvcSkeleton.h"
#include "../include/GatewayBasicConfig.h"
#include "Public/include/NetCenter.h"
namespace Zephyr
{

IMPLEMENT_START_HANDLE_INTERFACE(CGatewayService)
IMPLEMENT_HANDLE_INTERFACE(IfGatewaySvc)
IMPLEMENT_END_HANDLE_INTERFACE(CGatewayService)


CGatewayService::CGatewayService()
{
    m_pNet = NULL;
    m_uIp  = NULL;
    m_uListeningPort = NULL;
    m_nMaxConnections;
    m_uLastRoutineTime = 0;
}

TInt32 CGatewayService::Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV)
{
    return SUCCESS;
}
//注册服务 
TInt32 CGatewayService::RegisterService(TUInt32 uServiceId,TUInt32 uServicBegin,TUInt32 uEnd,TUInt32 uPriority,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}
//注销服务
TInt32 CGatewayService::UnRegisterService(TUInt32 uServiceId,TUInt32 uServicBegin,TUInt32 uEnd)
{
    return SUCCESS;
}
//发送广播聊天信息
TInt32 CGatewayService::BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}

TInt32 CGatewayService::ChangePriorty(TUInt32 uServiceId,CDoid *pMyDoid,TUInt32 uPriority)
{
    return SUCCESS;
}

//开始接收登陆，供管理使用。主控服务器在所有内部服务器协调启动完成后，让gateway开始接收登陆.
TInt32 CGatewayService::StartLogin(TUInt32 uIp,TUInt16 nListeningPort,TUInt16 nMaxConnection)
{
    return SUCCESS;
}
//停止接入登陆，供管理使用。开始停服.这个时候Gateway只是停止接收新的连接，老连接还是维持的
TInt32 CGatewayService::StopLogin()
{
    return SUCCESS;
}
//关闭所有客户端连接
TInt32 CGatewayService::DisconnectedAllClient()
{
    return SUCCESS;
}

    //以下是Service专有的.
TInt32 CGatewayService::OnInit()
{
    //根据ServiceID来获取配置
    CDoid *pDoid = GetMyDoid();
    //肯定有
    
    CGatewayBasicConfig tConfig;
    TInt32 nRet = tConfig.ReadFile(pDoid->m_virtualIp,pDoid->m_srvId);
    if (nRet < SUCCESS)
    {
        printf_s("Read config file failed!");
        return nRet;
    }
    //然后生成Net
    m_pNet = CreateNet(m_pTaskMgr,&m_tParserFactory,NULL,tConfig.m_uMaxIncomingConnection4Listner,
                        (tConfig.m_uOutPutCacheInKBs*1024),(tConfig.m_uInputCacheInKBs*1024));
    //然后生成日志
    nRet = m_pLoggerManager->AddLogger(tConfig.m_szLoggerName,-1,tConfig.m_uWriteLoggerMask,tConfig.m_uPrint2ScreenLoggerMask);
    if (nRet < SUCCESS)
    {
        printf("Create Logger Failed!");
        DestoryNet(m_pNet);
        return OUT_OF_MEM;
    }
    m_pLogger = m_pLoggerManager->GetLogger(nRet);
    m_pIfComm = m_pOrb->GetCommunicator();
    return SUCCESS;
}
    //结束是回调.
TInt32 CGatewayService::OnFinal()
{
    DestoryNet(m_pNet);
    m_pNet = NULL;
    m_pLoggerManager->ReleaseLogger(m_pLogger);
    m_pLogger = NULL;
    return SUCCESS;
}

    //定时  器到时了
TInt32 CGatewayService::OnTimer(TInt32 nTimerIdx,void *pData,TInt32 nTimeGap,TUInt32 nTimeNow)
{
    return SUCCESS;
}

    //以下是给Service使用的
    //定期的回调，可以注册循环时间，但只能有一个
TInt32 CGatewayService::OnRoutine(TUInt32 nRunCnt)
{
    TUInt32 nGap = GetClock()->GetTimeGap(m_uLastRoutineTime);
    if (nGap > 1000) //没1秒重新刷20个
    {
        m_uLastRoutineTime = GetClock()->GetLocalTime();
        TUInt32 uSize = m_tUsingSessions.size();
        if (uSize)
        {
            uSize >>= 7;
            ++uSize;
            while(uSize)
            {
                --uSize;
                CListNode<CGatewaySession> *pSession = m_tUsingSessions.pop_front();
                m_tUsingSessions.push_back(pSession);
                if (pSession)
                {
                    pSession->HeartBeat();
                }
                else
                {
                    break;
                }
            }
        }
    }
    return SUCCESS;
}
    //网络时间
TInt32 CGatewayService::OnNetEvent(CConnectionEvent *pEvent)
{
    return SUCCESS;
}

TInt32 CGatewayService::InitService(IfOrb* pOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr)
{
    m_pTaskMgr = pIfTaskMgr;
    m_pLoggerManager = pIfLoggerMgr;
    m_pOrb = pOrb;
    m_pClock = pOrb->GetClock();
    m_uLastRoutineTime = GetClock()->GetLocalTime();
    return SUCCESS;
}

IfConnectionCallBack *CGatewayService::OnNewConnection(CConPair *pPair)
{
    if (m_tUsingSessions.size() < m_nMaxConnections)
    {
        CListNode<CGatewaySession> *pMem = m_tSessionPool.GetMem();
        if (pMem)
        {
            m_tUsingSessions.push_front(pMem);
            return pMem;
        }
    }
    return NULL;
}

CDoid *CGatewayService::FindService(TUInt32 uServiceId)
{
    TUInt32 uSvr = (uServiceId>>11); //左移11位就可以了
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSvr);
    CRoute *pRount = it;
    CRoute *pRtn(NULL);
    while ((pRount)&&(pRount->m_uKey == uServiceId))
    {
        if ((uServiceId <= pRount->m_uIdBegin)&&(uServiceId> pRount->m_uIdEnd))
        {
            if (pRtn)
            {
                if (pRtn->m_uPriority > pRount->m_uPriority) //计算权重.
                {
                    pRtn = pRount;
                }
            }
            else
            {
                pRtn = pRount;
            }
        }
        ++it;
    }
    if (pRtn)
    {
        return &pRtn->m_tRouteTo;
    }
    return NULL;
}

TInt32 CGatewayService::AddRoute(CDoid *pDoid,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd,TUInt32 uPriority)
{
    
    if ((uSrvId != CMessageHeader::GetServiceID(uBegin))||(uSrvId!=CMessageHeader::GetServiceID(uEnd-1)))
    {
        //入参错误，写日志
        
        
        return FAIL;
    }
    CRoute *pRoute = m_tServiceRoute.PrepareItem();
    if (pRoute)
    {
        pRoute->m_uKey = CMessageHeader::GetServiceID(uSrvId);
        
    }
    else
    {
         
    }
    return SUCCESS;
}

CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr)
{
    CGatewayService *p = new CGatewayService();
    TInt32 nRet = p->InitService(pStubCenter,pIfTaskMgr,pIfLoggerMgr);
    if (nRet < SUCCESS)
    {
        delete p;
    }
    return NULL;
}

TInt32 ReleaseService(CService* pService)
{
    CGatewayService *pGatewayService = dynamic_cast<CGatewayService *>(pService);
    pGatewayService->OnFinal();
    if (pService)
    {
        return -1;
    }
    delete pGatewayService;
    return SUCCESS;
}


//查找默认的服务入口点，即这些服务不需要鉴权和注册也能拥有，比如登陆



}
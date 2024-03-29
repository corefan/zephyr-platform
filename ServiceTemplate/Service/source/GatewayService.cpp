#include "../include/GatewayService.h"
#include "Public/include/SysMacros.h"
#include "../../Interface/include/IfGatewaySvcSkeleton.h"
#include "../include/GatewayBasicConfig.h"
#include "Public/include/NetCenter.h"
#include "../../../DB/Interface/include/IfAuthServiceMethodId.h"
#include "../include/GatewayDefAndConst.h"
#include "../CryptLib2/rsaref.h"
#include "../CryptLib2/rsa.h"
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
    m_nMaxConnections = 0;
    m_uLastRoutineTime = 0;
    m_pListener = NULL;
}

TInt32 CGatewayService::Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV)
{
    return SUCCESS;
}
//注册服务 
TInt32 CGatewayService::RegisterService(TUInt32 uServiceId,TUInt32 uServicBegin,TUInt32 uEnd,TUInt32 uPriority,OctSeq<TUInt16>& tServiceName)
{
    AddRoute(GetCallerDoid(),uServiceId,uServicBegin,uEnd,uPriority);
    
    return SUCCESS;//表示处理了.
}
//注销服务
TInt32 CGatewayService::UnRegisterService(TUInt32 uServiceId,TUInt32 uServicBegin,TUInt32 uEnd)
{
    CDoid *pRegister = GetCallerDoid();
    TInt32 nRet = m_tServiceRoute.RmvRoute(pRegister,uServiceId,uServicBegin,uEnd);
    if (nRet < SUCCESS)
    {
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        if (nRet < en_gw_error_begin)
        {
            LOG_RUN((-nRet),"UnRegister from:%s , uSrvId:%u,uBegin:%u,uEnd:%u,",szBufferRegister,uServiceId,uServicBegin,uEnd);
        }
        else
        {
            LOG_CRITICAL((-nRet),"UnRegister from :%s , uSrvId:%u,uBegin:%u,uEnd:%u,",szBufferRegister,uServiceId,uServicBegin,uEnd);
        }
    }
    return SUCCESS;
}
//发送广播聊天信息
TInt32 CGatewayService::BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}

TInt32 CGatewayService::ChangePriorty(TUInt32 uServiceId,CDoid *pMyDoid,TUInt32 uPriority)
{
    TInt32 nRet = m_tServiceRoute.ChangePriorty(uServiceId,pMyDoid,uPriority);
    if (0 == nRet)
    {
        CDoid *pRegister = GetCallerDoid();
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        char szBufferDoid[64];
        pMyDoid->ToStr(szBufferDoid);
        LOG_RUN((-nRet),"Register:%s ,doid:%s, uSrvId:%u,uPriority:%u",szBufferRegister,szBufferDoid,uServiceId,uPriority);
    }
    return nRet;
}

//开始接收登陆，供管理使用。主控服务器在所有内部服务器协调启动完成后，让gateway开始接收登陆.
TInt32 CGatewayService::StartLogin(TUInt32 uIp,TUInt16 nListeningPort,TUInt16 nMaxConnection)
{
    if (NULL == m_pListener)
    {
        if ((0 == m_uListeningPort)&&(0 == m_uIp))
        {
            IfListenerCallBack *pCallBack = this; //必须转一下，不位不对，Listen的参数void*,不是IfListenerCallBack*
            char szBuffer[64];
            GetCallerDoid()->ToStr(szBuffer);
            LOG_RUN(en_start_listing,"Caller(%s) start Listening at Ip:%u,port:%u,MaxConnection:%u",szBuffer,uIp,(TUInt32)nListeningPort,(TUInt32)nMaxConnection);

            m_pListener = m_pNet->Listen(uIp,nListeningPort,nMaxConnection,pCallBack);

            if (NULL == m_pListener)
            {
                LOG_RUN(en_start_listening_failed,"Listen failed! Ip:%u,port:%u",uIp,(TUInt32)nListeningPort);
                return -(en_start_listening_failed);
            }
        }
        else
        {
            //重复启动
            CDoid *pRegister = GetCallerDoid();
            char szBufferRegister[64];
            pRegister->ToStr(szBufferRegister);
            LOG_RUN(en_restart_listening,"Restart listening from doid:%s , New Ip:%u,port:%u,max connection:%u",szBufferRegister,uIp,(TUInt32)nListeningPort,(TUInt32)nMaxConnection);
        }
    }
    return SUCCESS;
}
//停止接入登陆，供管理使用。开始停服.这个时候Gateway只是停止接收新的连接，老连接还是维持的
TInt32 CGatewayService::StopLogin()
{
    if (m_pListener)
    {
        char szBuffer[64];
        GetCallerDoid()->ToStr(szBuffer);
        if (m_pListener)
        {
            LOG_RUN(en_stop_listening,"Caller(%s) stop Listening",szBuffer);
            m_pNet->StopListening(m_pListener); //肯定成功.
            m_pListener = NULL;
        }
        else
        {
            LOG_RUN(en_listening_not_started,"Caller(%s) try stop Listening,but there's no listening.",szBuffer);
        }
    }
    return SUCCESS;
}
//关闭所有客户端连接
TInt32 CGatewayService::DisconnectedAllClient()
{
    return SUCCESS;
}

void CGatewayService::OnDisconnected(CGatewaySession *pSession,IfParser *pParser,IfCryptor *pCryptor,TInt32 uReason)
{
    //pParser \ pCryptor都不用释放.
    LOG_RUN(en_on_disconnected,"OnDisconnected,UserId:%llu,SystemId:%u,Reason%d",pSession->GetUserId(),pSession->GetSystemId(),uReason);

    CListNode<CGatewaySession> *pListNode = (CListNode<CGatewaySession> *)pSession;
    if (pSession->m_uStartDisconnectTime) //this is delayedd release
    {
        m_tWaitingDisconnect.Detach(pListNode);
    }
    else
    {
        m_tUsingSessions.Detach(pListNode);
    }
    m_pOrb->UnRegisterObj(pSession->GetSkeleton());
    
    //pSession->OnFinal();
    m_tSessionPool.ReleaseMem(pListNode);
}

    //以下是Service专有的.
TInt32 CGatewayService::OnInited()
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
    m_tParserFactory.InitFarctory(tConfig.m_uMaxConnections);
    m_pNet = CreateNet(m_pTaskMgr,&m_tParserFactory,&m_tParserFactory,tConfig.m_uMaxConnections,
                        (tConfig.m_uOutPutCacheInKBs*1024),(tConfig.m_uInputCacheInKBs*1024));
    if (!m_pNet)
    {
        printf("Create net module failed!");
        return FAIL;
    }
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
    m_pOrb->RegisterRun(m_pSkeleton,15);
    //临时的
    IfListenerCallBack *pCallback = this;
    m_pListener = m_pNet->Listen(tConfig.m_uListeningIp,tConfig.m_uListeningPort,tConfig.m_uMaxIncomingConnection4Listner,pCallback);
    //
    
    m_nMaxConnections = tConfig.m_uMaxConnections;
    nRet = m_tRoutePool.InitPool(m_nMaxConnections*4);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    m_tServiceRoute.Init(&m_tRoutePool);
    m_tServiceRoute.AddRoute(&tConfig.m_tASDoid,IfAuthServiceServiceId,IfAuthServiceServiceIdBegin,IfAuthServiceServiceIdEnd+1,0);
    m_tSessionPool.InitPool(m_nMaxConnections);
    if (NULL == m_pListener)
    {
        return FAIL;
    }
    R_RANDOM_STRUCT randomStruct;
    R_RSA_PROTO_KEY protoKey;
    int status;
    /* Initialise random structure ready for keygen */
    R_RandomCreate(&randomStruct);
    /* Initialise prototype key structure */
    protoKey.bits=1024;
    protoKey.useFermat4 = 1;
    /* Generate keys */
    status = R_GeneratePEMKeys(&m_tPublicKey, &m_tPrivateKey, &protoKey, &randomStruct);
    if (status)
    {
        printf("R_GeneratePEMKeys failed with %d\n", status);
        return FAIL;
    }
    if (m_tClientSkeleton.InitMem((tConfig.m_uOutPutCacheInKBs*1024)) < SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
    //
}
    //结束是回调.
void CGatewayService::OnFinal()
{
    DestoryNet(m_pNet);
    m_pNet = NULL;
    m_pLoggerManager->ReleaseLogger(m_pLogger);
    m_pLogger = NULL;
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
    int nRet = m_pNet->Run(nRunCnt);
    TUInt32 nGap = GetClock()->GetTimeGap(m_uLastRoutineTime);
    if (nGap > 1000) //1秒重新刷20个
    {
        m_uLastRoutineTime = GetClock()->GetLocalTime();
        TUInt32 uSize = m_tUsingSessions.size();
        if (uSize)
        {
            uSize >>= 7;
            ++uSize;
            while(uSize)
            {
                ++nRet;
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
    CGatewaySession *pFirst = m_tWaitingDisconnect.header();
    if (pFirst && (pFirst->m_uStartDisconnectTime > m_pClock->GetTimeInSec()))
    {
        //断开
        pFirst->DelayedDisconnect();
    }

    return nRet;
}

//U must detach pSession from the prev list first by yourself.
void CGatewayService::Wait2Disconnect(CGatewaySession *pSession)
{
    if (0 == pSession->m_uStartDisconnectTime)
    {
        CListNode<CGatewaySession> *pListNode = (CListNode<CGatewaySession>*)pSession;
        pSession->m_uStartDisconnectTime = m_pClock->GetTimeInSec() + WAIT_DISCONNECT_TIME;
        m_tUsingSessions.Detach(pListNode);
        m_tWaitingDisconnect.push_back(pListNode);
    }
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
    R_RandomCreate(&m_tRandStruct);
    R_RSA_PROTO_KEY  tProtoKey;
    tProtoKey.bits = 1024;
    tProtoKey.useFermat4 = 1;

    memset(&m_tPublicKey,0,sizeof(m_tPublicKey));
    memset(&m_tPrivateKey,0,sizeof(m_tPrivateKey));

    int nRet = R_GeneratePEMKeys(&m_tPublicKey, &m_tPrivateKey,&tProtoKey,&m_tRandStruct);
    char demostring[] = "Test string for RSA functions #1";
    char encryptedString[MAX_RSA_MODULUS_LEN+2];
    char decryptedString[256];
    int status;
    unsigned int encryptedLength, decryptedLength;

    status = RSAPrivateEncrypt((unsigned char*)encryptedString, &encryptedLength,(unsigned char*) demostring,
        strlen(demostring)+1, &m_tPrivateKey);
    if (status)
    {
        printf("RSAPrivateEncrypt failed with %x\n", status);
        return SUCCESS;
    }
    /* Decrypt with public key */
    status = RSAPublicDecrypt((unsigned char*)decryptedString, &decryptedLength,(unsigned char*) encryptedString,
        encryptedLength, &m_tPublicKey);
    if (status)
    {
        printf("RSAPublicDecrypt failed with %x\n", status);
        return SUCCESS;
    }
    /* Display decrypted string */
    decryptedString[decryptedLength+1] = (char) "\0";
    printf("Decrypted string: %s\n", decryptedString);

    return SUCCESS;
}

IfConnectionCallBack *CGatewayService::OnNewConnection(CConPair *pPair)
{
    if (m_tUsingSessions.size() < m_nMaxConnections)
    {
        CListNode<CGatewaySession> *pMem = m_tSessionPool.GetMem();
        if (pMem)
        {
            pMem->OnInit();
            pMem->Init(this);
            IfSkeleton *pSkeleton =  m_pOrb->RegisterObj(pMem,m_nServiceId);
            if (pSkeleton)
            {
                pMem->SetSkeleton(pSkeleton);
            }
            else
            {
                m_tSessionPool.ReleaseMem(pMem);
                return NULL;
            }
            m_tUsingSessions.push_front(pMem);
            return pMem;
        }
    }
    return NULL;
}

CDoid *CGatewayService::FindService(TUInt32 uServiceId)
{
    return m_tServiceRoute.FindService(uServiceId);
}

TInt32 CGatewayService::AddRoute(CDoid *pDoid,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd,TUInt32 uPriority)
{
    TInt32 nRet = m_tServiceRoute.AddRoute(pDoid,uSrvId,uBegin,uEnd,uPriority);
    if (nRet < SUCCESS)
    {
        CDoid *pRegister = GetCallerDoid();
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        char szBufferDoid[64];
        pDoid->ToStr(szBufferDoid);
        if (nRet < en_gw_error_begin)
        {
            LOG_RUN((-nRet),"Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
        }
        else
        {
            LOG_CRITICAL((-nRet),"Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
        }
    }
    return nRet;
}

CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr)
{
    CGatewayService *p = new CGatewayService();
    TInt32 nRet = p->InitService(pStubCenter,pIfTaskMgr,pIfLoggerMgr);
    if (nRet < SUCCESS)
    {
        delete p;
        p = NULL;
    }
    return p;
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
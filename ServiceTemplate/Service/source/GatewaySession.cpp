#include "../include/GatewaySession.h"
#include "../../Interface/include/IfConnectingSkeleton.h"
#include "../include/GatewayService.h"
#include "../../Interface/include/IfConnectingRespStub.h"
#include <time.h>
#pragma warning(push)
#pragma warning(disable:4244)

namespace Zephyr
{

IMPLEMENT_START_HANDLE_INTERFACE(CGatewaySession)
IMPLEMENT_HANDLE_INTERFACE(IfConnecting)
IMPLEMENT_END_HANDLE_INTERFACE(CGatewaySession)


CGatewaySession::CGatewaySession()
{
    OnInit();
}

void CGatewaySession::Init(CGatewayService *pService)
{
    m_pService = pService;
    m_tServiceRoute.Init(pService->GetRoutePool());
    m_uLastOprTime = pService->GetClock()->GetLocalTime();
}

void CGatewaySession::OnConnected(TUInt32 uIp,TUInt16 uPortId)
{
    m_uIp = uIp;
    m_uPort = uPortId;
    m_enState = en_connection_established;
}

TInt32 CGatewaySession::OnRecv(TUChar *pMsg, TUInt32 msgLen)
{
    OnNetIO();
    CMessageHeader::UnMsgInfo *pMsgInfo = (CMessageHeader::UnMsgInfo *)pMsg;
    CMessageHeader::MsgInfoC2S(*pMsgInfo);
    TUInt32 uMsgId = pMsgInfo->m_methodId;
    if (pMsgInfo->m_msgBodyLength + sizeof(CMessageHeader::UnMsgInfo) == msgLen) //这个是肯定的
    {
        if (HandleClientMsg(pMsgInfo))
        {
            return msgLen;
        }
        CDoid *pDoid = m_tServiceRoute.FindService(uMsgId);
        if (NULL == pDoid)
        {
            pDoid = m_pService->FindService(uMsgId);
        }
        if (pDoid)
        {
            CMessageHeader *pMsgHeader = PrepareMsg(pMsgInfo->m_msgBodyLength,uMsgId,pDoid,1,false);
            if (pMsg)
            {
                memcpy(pMsgHeader->GetBody(),(pMsg+sizeof(CMessageHeader::UnMsgInfo)),pMsgInfo->m_msgBodyLength);
                return SendMsg(pMsgHeader);
            }
        }
    }
    return msgLen;
}

TBOOL  CGatewaySession::HandleClientMsg(CMessageHeader::UnMsgInfo *pMsgInfo)
{
    if (20 == pMsgInfo->m_methodId)
    {
        //回发一条消息。
        TUChar szBufferRtn[12];
        CMessageHeader::UnMsgInfo *pMsgInfo = (CMessageHeader::UnMsgInfo *)szBufferRtn;
        pMsgInfo->m_uMsgLength = 4;
        pMsgInfo->m_uMsgId = 20;
        if(m_pIfConnection->SendMsg(szBufferRtn,12))
        {
            return True;
        }
    }
    return False;
}

//redirct 2 client;
TInt32 CGatewaySession::OnRecvUnacceptableMsg(CMessageHeader *pMsg)
{
    OnNetIO();
    return SUCCESS;
}
    //virtual TInt32 OnRecvIn2Piece(TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
    //网络层会自动从factory生成parser和crypter,请应用层对这连个东西进行设置
    //应用层应该明确知道IfParser 和 IfCryptor的实现类，并在OnConnected的时候对其进行设置.
    //如果返回值en_if_connection_callback_on_connected_release_parser 设置了，那么parser会被释放
    //如果返回值en_if_connection_callback_on_connected_release_cryptor 设置了，那么pCryptor会被释放
    //IfConnection *pIfConnection在连接实际建立的时候再传给应用层。
TInt32 CGatewaySession::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    m_pIfConnection = pIfConnection;
    m_pParser = pParser;
    m_pCryptor = pCryptor;
    CConPair *pConn = pIfConnection->GetConnectionInfo();
    OnConnected(pConn->GetMyIp(),pConn->GetMyPort());
    return SUCCESS;
}
    //任何socket异常都会自动关闭网络连接
TInt32 CGatewaySession::OnDissconneted(TInt32 nErrorCode)
{
    m_pIfConnection = NULL;
    if (nErrorCode != en_disconnect_without_reply)
    {
        m_tServiceRoute.ReleaseAndInfoRegister(GetSkeleton(),nErrorCode);
    }
    else
    {
        m_tServiceRoute.OnFinal(); //release the route map!
    }
    //注意，这个操作之后，可能内存即释放了，不能再做其它操作.
    m_pService->OnDisconnected(this,m_pParser,m_pCryptor,nErrorCode);
    return SUCCESS;
}

    //在初始化的时候会被调.
TInt32 CGatewaySession::OnInit()
{
    m_uUserId = 0;
    m_uSystemId = 0;
    m_uLastOprTime = 0;
    m_enState = en_connection_not_using;
    m_uIp = 0;
    m_uPort = 0;
    m_pIfConnection = NULL;
    m_pParser = NULL;
    m_pCryptor = NULL;
    m_uStartDisconnectTime = 0;
    return SUCCESS;
}
    //结束是回调.
void CGatewaySession::OnFinal()
{
    m_tServiceRoute.OnFinal();
    m_enState = en_connection_not_using;
    m_uIp = 0;
    m_uPort = 0;
    m_pIfConnection = NULL;
    m_pParser = NULL;
    m_pCryptor = NULL;
    m_uStartDisconnectTime = 0;
}

TInt32 CGatewaySession::RegisterService(CDoid *pDoid,TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,TUInt32 uPriority)
{
    TInt32 nRet = m_tServiceRoute.AddRoute(pDoid,uServiceId,uServiceIdBegin,uServcieIdEnd,uPriority);
    if (nRet < SUCCESS)
    {
        CDoid *pRegister = GetCallerDoid();
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        char szBufferDoid[64];
        pDoid->ToStr(szBufferDoid);
        if (nRet < en_gw_error_begin)
        {
            LOG_RUN((-nRet),"Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uServiceId,uServiceIdBegin,uServcieIdEnd,uPriority);
        }
        else
        {
            LOG_CRITICAL((-nRet),"Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uServiceId,uServiceIdBegin,uServcieIdEnd,uPriority);
        }
    }
    IfConnectingResp *pResp;
    GET_REMOTE_CALLER(pResp,IfConnectingResp);
    pResp->RespRegisterService(pDoid,uServiceId,nRet);
    return nRet;
}

TInt32 CGatewaySession::UnregisterService(TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd)
{
    CDoid *pRegister = GetCallerDoid();
    TInt32 nRet = m_tServiceRoute.RmvRoute(pRegister,uServiceId,uServiceIdBegin,uServcieIdEnd);
    if (nRet < SUCCESS)
    {
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        if (nRet < en_gw_error_begin)
        {
            LOG_RUN((-nRet),"UnRegister from:%s , uSrvId:%u,uBegin:%u,uEnd:%u,",szBufferRegister,uServiceId,uServiceIdBegin,uServcieIdEnd);
        }
        else
        {
            LOG_CRITICAL((-nRet),"UnRegister from :%s , uSrvId:%u,uBegin:%u,uEnd:%u,",szBufferRegister,uServiceId,uServiceIdBegin,uServcieIdEnd);
        }
    }
    IfConnectingResp *pResp;
    GET_REMOTE_STUB_PT(pResp,IfConnectingResp,pRegister);
    pResp->ConfirmUnregisterService(uServiceId);
    return SUCCESS;
}
    //注册广播
TInt32 CGatewaySession::RegisterTeam(TUInt32 uTeamID)
{
    return SUCCESS;
}

TInt32 CGatewaySession::Disconnect(TUInt32 uReason)
{
    if (en_disconnect_with_delay == uReason)
    {
        m_pService->Wait2Disconnect(this);
    }
    else
    {
        m_pIfConnection->Disconnect();
        OnDissconneted(uReason);
    }
    return SUCCESS;
}

void CGatewaySession::DelayedDisconnect()
{
    m_pIfConnection->Disconnect();
    OnDissconneted(en_direct_dis);
}

TInt32 CGatewaySession::SetId(TUInt32 uId)
{
    m_uSystemId = uId;
    return SUCCESS;
}

TInt32 CGatewaySession::CheckId(TUInt32 uId)
{
    if (m_uSystemId != uId)
    {
        LOG_RUN(en_system_id_unmatch,"System Id incorrect! local system Id :%u, test id:%u",m_uSystemId,uId);
        Disconnect((-en_system_id_unmatch));
    }
    return SUCCESS;
}


void CGatewaySession::HeartBeat()
{

    TUInt32 nGap = m_pService->GetClock()->GetTimeGap(m_uLastOprTime);
    if (nGap > 30000) //心跳时间30秒
    {
        SendHeartBeat();
        m_uLastOprTime = m_pService->GetClock()->GetLocalTime();
    }
}
void CGatewaySession::SendHeartBeat()
{

}

void CGatewaySession::OnNetIO()
{
    m_uLastOprTime = m_pService->GetClock()->GetLocalTime();
}

IfLogger *CGatewaySession::GetLogger()
{
#ifdef _DEBUG
    if (NULL == m_pService)
    {
        return NULL;
    }
#endif
    return m_pService->GetLogger();
}

TInt32 CGatewaySession::SendMsg2Client(CMessageHeader *pMsg)
{
    if (m_pIfConnection)
    {
        TInt32 nBuffLen = m_pIfConnection->GetFreeBuffLength();
        TInt32 nCompactLen = pMsg->GetCompackedLen();
        if (nBuffLen > pMsg->GetCompackedLen())
        {
            return m_pIfConnection->SendMsg(pMsg->GetCompactedBody(),nCompactLen);
        }
    }
    return OUT_OF_MEM_BUFFER;
}
}
#pragma warning(pop)

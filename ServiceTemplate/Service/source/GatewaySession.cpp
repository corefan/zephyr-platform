#include "../include/GatewaySession.h"
#include "../../Interface/include/IfConnectingSkeleton.h"
#include "../include/GatewayService.h"
#include "../../Interface/include/IfConnectingRespStub.h"
#include <time.h>
#include "../../../DB/Interface/include/IfAuthServiceMethodId.h"
#include "../../Interface/include/IfConnectingRespSkeleton.h"
#include "../CryptLib2/rsa.h"
#include "../include/GatewayParser.h"

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
    if (pMsgInfo->m_msgBodyLength + sizeof(CMessageHeader::UnMsgInfo) == msgLen) //����ǿ϶���
    {
        if (HandleClientMsg(pMsgInfo)>=SUCCESS)
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

TInt32  CGatewaySession::HandleClientMsg(CMessageHeader::UnMsgInfo *pMsgInfo)
{
    CMessageHeader *pMsg = GET_CONTAINER(pMsgInfo,CMessageHeader,m_msgInfo);
    IMPLEMENT_HANDLE_INTERFACE(IfConnecting);
    return CAN_NOT_HANDLE_THIS_MSG;
}

//redirct 2 client;
TInt32 CGatewaySession::OnRecvUnacceptableMsg(CMessageHeader *pMsg)
{
    OnNetIO();
    CClientSkeleton *pClientSkeleton = m_pService->GetSkeleton();
    pClientSkeleton->Reset(m_pIfConnection);
    pClientSkeleton->SendMsg(pMsg);
    return SUCCESS;
}

    //virtual TInt32 OnRecvIn2Piece(TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
    //�������Զ���factory����parser��crypter,��Ӧ�ò��������������������
    //Ӧ�ò�Ӧ����ȷ֪��IfParser �� IfCryptor��ʵ���࣬����OnConnected��ʱ������������.
    //�������ֵen_if_connection_callback_on_connected_release_parser �����ˣ���ôparser�ᱻ�ͷ�
    //�������ֵen_if_connection_callback_on_connected_release_cryptor �����ˣ���ôpCryptor�ᱻ�ͷ�
    //IfConnection *pIfConnection������ʵ�ʽ�����ʱ���ٴ���Ӧ�ò㡣
TInt32 CGatewaySession::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    m_pIfConnection = pIfConnection;
    m_pParser = pParser;
    m_pCryptor = pCryptor;
    CConPair *pConn = pIfConnection->GetConnectionInfo();
    CClientSkeleton *pSkeleton = m_pService->GetSkeleton();
    pSkeleton->Reset(m_pIfConnection);
    IfConnectingRespStub tClientStub;
    tClientStub.Init(pSkeleton,pSkeleton->GetMyDoid());
    OctSeq<TUInt16> e,n;
    e.m_nBodyLength = 3;
    e.m_pBuffer = &m_pService->GetPublicKey()->exponent[MAX_RSA_MODULUS_LEN-3];
    n.m_nBodyLength = MAX_RSA_MODULUS_LEN;
    n.m_pBuffer = m_pService->GetPublicKey()->modulus;
    tClientStub.SendRSAPublicKey(1024,1,e,n,0);
    OnConnected(pConn->GetMyIp(),pConn->GetMyPort());
    return SUCCESS;
}
    //�κ�socket�쳣�����Զ��ر���������
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
    //ע�⣬�������֮�󣬿����ڴ漴�ͷ��ˣ�����������������.
    m_pService->OnDisconnected(this,m_pParser,m_pCryptor,nErrorCode);
    return SUCCESS;
}

    //�ڳ�ʼ����ʱ��ᱻ��.
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
    //�����ǻص�.
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
    //ע��㲥
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
    if (nGap > 30000) //����ʱ��30��
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

TInt32 CGatewaySession::SendCryptedKey(OctSeq<TUInt32> tKey,TUInt32 uPadding)
{
    if (128 == tKey.m_nBodyLength)
    {
        //����
        TUChar szOrinal[256];
        unsigned int decryptedLength=0;

        int status = RSAPrivateDecrypt(szOrinal, &decryptedLength,
            tKey.m_pBuffer, tKey.m_nBodyLength, m_pService->GetPrivateKey());
        if (0 < status)
        {
            printf("Decrypt failed!");
        }
        if (64 == decryptedLength)
        {
            CGatewayParser *pParser = (CGatewayParser*)m_pParser;
            pParser->SetKey((TUInt16*)szOrinal);
        }
    }
    return SUCCESS;
}

}
#pragma warning(pop)

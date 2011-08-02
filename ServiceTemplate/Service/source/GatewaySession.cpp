#include "../include/GatewaySession.h"
#include "../../Interface/include/IfConnectingSkeleton.h"
#include "../include/GatewayService.h"

namespace Zephyr
{

IMPLEMENT_START_HANDLE_INTERFACE(CGatewaySession)
IMPLEMENT_HANDLE_INTERFACE(IfConnecting)
IMPLEMENT_END_HANDLE_INTERFACE(CGatewaySession)


CGatewaySession::CGatewaySession()
{

}

void CGatewaySession::Init(CGatewayService *pService)
{
    m_pService = pService;
    m_tServiceRoute.Init(pService->GetRoutePool(),pService->GetLogger());
    m_uLastOprTime = pService->GetClock()->GetLocalTime();
}

void CGatewaySession::OnConnected(TUInt32 uIp,TUInt16 uPortId)
{

}

TInt32 CGatewaySession::OnRecv(TUChar *pMsg, TUInt32 msgLen)
{
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
    return SUCCESS;
}
    //�κ�socket�쳣�����Զ��ر���������
TInt32 CGatewaySession::OnDissconneted(TInt32 erroCode)
{
    return SUCCESS;
}

    //�ڳ�ʼ����ʱ��ᱻ��.
TInt32 CGatewaySession::OnInit()
{
    return SUCCESS;
}
    //�����ǻص�.
TInt32 CGatewaySession::OnFinal()
{
    m_tServiceRoute.OnFinal();
    return SUCCESS;
}

TInt32 CGatewaySession::RegisterService(CDoid *pDoid,TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,TUInt32 uPriority)
{
    return SUCCESS;
}

TInt32 CGatewaySession::UnregisterService(TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd)
{
    return SUCCESS;
}
    //ע��㲥
TInt32 CGatewaySession::RegisterTeam(TUInt32 uTeamID)
{
    return SUCCESS;
}

TInt32 CGatewaySession::Disconnect(TUInt32 uReason)
{
    return SUCCESS;
}
TInt32 CGatewaySession::SetId(TUInt32 uId)
{
    return SUCCESS;
}

TInt32 CGatewaySession::CheckId(TUInt32 uId)
{
    return SUCCESS;
}
    //Ĭ����0Ȩ�ޣ���ߣ����Ȳ���Ļ���ʹ��
TInt32 CGatewaySession::AddRoute(CDoid *pDoid,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd,TUInt32 uPriority)
{

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

}

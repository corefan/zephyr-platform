#include "../include/GatewayService.h"
#include "Public/include/SysMacros.h"
#include "../../Interface/include/IfGatewaySvcSkeleton.h"

namespace Zephyr
{

IMPLEMENT_START_HANDLE_INTERFACE(CGatewayService)
IMPLEMENT_HANDLE_INTERFACE(IfGatewaySvc)
IMPLEMENT_END_HANDLE_INTERFACE(CGatewayService)

TInt32 CGatewayService::Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV)
{
    return SUCCESS;
}
    //ע�����
TInt32 CGatewayService::RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}
    //ע������
TInt32 CGatewayService::UnRegisterService(TUInt32 uServiceId)
{
    return SUCCESS;
}
    //���͹㲥������Ϣ
TInt32 CGatewayService::BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}


    //������Serviceר�е�.
TInt32 CGatewayService::OnInit()
{
    return SUCCESS;
}
    //�����ǻص�.
TInt32 CGatewayService::OnFinal()
{
    return SUCCESS;
}

    //��ʱ  ����ʱ��
TInt32 CGatewayService::OnTimer(TInt32 nTimerIdx,void *pData,TInt32 nTimeGap,TUInt32 nTimeNow)
{
    return SUCCESS;
}

    //�����Ǹ�Serviceʹ�õ�
    //���ڵĻص�������ע��ѭ��ʱ�䣬��ֻ����һ��
TInt32 CGatewayService::OnRoutine(TUInt32 nRunCnt)
{
    return SUCCESS;
}
    //����ʱ��
TInt32 CGatewayService::OnNetEvent(CConnectionEvent *pEvent)
{
    return SUCCESS;
}

CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr)
{
    CGatewayService *p = new CGatewayService();
    return p;
}

TInt32 ReleaseService(CService* pService)
{
    CGatewayService *pGatewayService = dynamic_cast<CGatewayService *>(pService);
    if (pService)
    {
        return -1;
    }
    delete pGatewayService;
    return SUCCESS;
}


}
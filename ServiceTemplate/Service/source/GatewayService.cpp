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
    //注册服务
TInt32 CGatewayService::RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}
    //注销服务
TInt32 CGatewayService::UnRegisterService(TUInt32 uServiceId)
{
    return SUCCESS;
}
    //发送广播聊天信息
TInt32 CGatewayService::BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName)
{
    return SUCCESS;
}


    //以下是Service专有的.
TInt32 CGatewayService::OnInit()
{
    return SUCCESS;
}
    //结束是回调.
TInt32 CGatewayService::OnFinal()
{
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
    return SUCCESS;
}
    //网络时间
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
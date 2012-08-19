#ifndef __IFGATEWAYRESP_SKELETON_H__
#define __IFGATEWAYRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfGatewayResp.h"
#include "IfGatewayRespMethodId.h"
namespace Zephyr 
{
class IfGatewayRespSkeleton 
{
public:
    IfGatewayResp *m_pImplementObj;
    IfGatewayRespSkeleton(IfGatewayResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfGatewayRespServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleConfirmRegisterService_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleConfirmUnRegisterService_TUInt32(CMessageHeader *pMsg);
};
}
#endif

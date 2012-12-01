#ifndef __IFCONNECTINGRESP_SKELETON_H__
#define __IFCONNECTINGRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfConnectingResp.h"
#include "IfConnectingRespMethodId.h"
namespace Zephyr 
{
class IfConnectingRespSkeleton 
{
public:
    IfConnectingResp *m_pImplementObj;
    IfConnectingRespSkeleton(IfConnectingResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfConnectingRespServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleRespRegisterService_CDoid_pt_TUInt32_TInt32(CMessageHeader *pMsg);
    TInt32 HandleConfirmUnregisterService_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleOnDisconnect_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleSendRSAPublicKey_TUInt16_TUInt16_OctSeq_tpl_begin_TUInt16_tpl_end__OctSeq_tpl_begin_TUInt16_tpl_end__TUInt32(CMessageHeader *pMsg);
};
}
#endif

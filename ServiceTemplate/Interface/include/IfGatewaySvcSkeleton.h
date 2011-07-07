#ifndef __IFGATEWAYSVC_SKELETON_H__ 
#define __IFGATEWAYSVC_SKELETON_H__
#include "../interface/IfGateway.h"
namespace Zephyr 
{
class IfGatewaySvcSkeleton 
{
public:
    IfGatewaySvc *m_pImplementObj;
    IfGatewaySvcSkeleton(IfGatewaySvc *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBool  IsMine(CMessageHeader *pMsg); //是否属于这个接口
    TInt32 HandleSyn2Map_TUInt32_TLV_tpl_begin_TUInt8_and_TUInt16_tpl_end__ref(CMessageHeader *pMsg);;
    TInt32 HandleRegisterService_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref(CMessageHeader *pMsg);;
    TInt32 HandleUnRegisterService_TUInt32(CMessageHeader *pMsg);;
    TInt32 HandleBroadcastTeamMsg_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref(CMessageHeader *pMsg);;
};
}
#endif

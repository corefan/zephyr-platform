#ifndef __IFAUTHRESP_SKELETON_H__
#define __IFAUTHRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfAuthResp.h"
#include "IfAuthRespMethodId.h"
namespace erp_platform 
{
class IfAuthRespSkeleton 
{
public:
    IfAuthResp *m_pImplementObj;
    IfAuthRespSkeleton(IfAuthResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfAuthRespServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleRespAuthenticate_TInt32_TUInt64(CMessageHeader *pMsg);
    TInt32 HandleConfirmDisconneted_CDoid(CMessageHeader *pMsg);
};
}
#endif

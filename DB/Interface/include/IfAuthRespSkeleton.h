#ifndef __IFAUTHRESP_SKELETON_H__
#define __IFAUTHRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfAuthResp.h"
#include "IfAuthRespMethodId.h"
namespace Zephyr 
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
         return ((IFAUTHRESP_INTERFACE_ID)&(pMsg->GetMethodId()));
     }
    TInt32 HandleRespAuthenticate_TInt32_TLV_tpl_begin_TUInt16_and_TUInt16_tpl_end_(CMessageHeader *pMsg);
};
}
#endif

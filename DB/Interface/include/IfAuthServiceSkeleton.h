#ifndef __IFAUTHSERVICE_SKELETON_H__
#define __IFAUTHSERVICE_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfAuthService.h"
#include "IfAuthServiceMethodId.h"
namespace Zephyr 
{
class IfAuthServiceSkeleton 
{
public:
    IfAuthService *m_pImplementObj;
    IfAuthServiceSkeleton(IfAuthService *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
     {
         return ((IFAUTHSERVICE_INTERFACE_ID)&(pMsg->GetMethodId()));
     }
    TInt32 HandleAuthenticate_TLV_tpl_begin_TUInt16_and_TUInt16_tpl_end_(CMessageHeader *pMsg);
};
}
#endif

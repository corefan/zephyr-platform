#ifndef __IFAUTHSERVICE_SKELETON_H__
#define __IFAUTHSERVICE_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/ifauthService.h"
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
         return ((IfAuthServiceServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleAuthenticate_TUInt32_TChar_pt_TChar_pt(CMessageHeader *pMsg);
    TInt32 HandleOnDisconneted_CDoid(CMessageHeader *pMsg);
};
}
#endif

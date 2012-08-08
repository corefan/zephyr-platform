#ifndef __IFLOGICSERVICE_SKELETON_H__
#define __IFLOGICSERVICE_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfLogicService.h"
#include "IfLogicServiceMethodId.h"
namespace test_logic 
{
class IfLogicServiceSkeleton 
{
public:
    IfLogicService *m_pImplementObj;
    IfLogicServiceSkeleton(IfLogicService *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
     {
         return ((IFLOGICSERVICE_INTERFACE_ID)==(pMsg->GetMethodId()));
     }
    TInt32 HandleReqGetSession_TUInt64_CDoid(CMessageHeader *pMsg);
    TInt32 HandleReqReleaseSession_TUInt64_CDoid(CMessageHeader *pMsg);
};
}
#endif

#ifndef __IFLOGICSERVICERESP_SKELETON_H__
#define __IFLOGICSERVICERESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/iflogicServiceResp.h"
#include "IfLogicServiceRespMethodId.h"
namespace test_logic 
{
class IfLogicServiceRespSkeleton 
{
public:
    IfLogicServiceResp *m_pImplementObj;
    IfLogicServiceRespSkeleton(IfLogicServiceResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //�Ƿ���������ӿ�
     {
         return ((IFLOGICSERVICERESP_INTERFACE_ID)==(pMsg->GetMethodId()));
     }
    TInt32 HandleRespGetSession_TUInt64_CDoid(CMessageHeader *pMsg);
    TInt32 HandleRespReleaseSession_TUInt64(CMessageHeader *pMsg);
};
}
#endif

#ifndef __IFLOGICRESP_SKELETON_H__
#define __IFLOGICRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/iflogicResp.h"
#include "IfLogicRespMethodId.h"
namespace test_logic 
{
class IfLogicRespSkeleton 
{
public:
    IfLogicResp *m_pImplementObj;
    IfLogicRespSkeleton(IfLogicResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfLogicRespServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleRespTest_TInt32(CMessageHeader *pMsg);
};
}
#endif

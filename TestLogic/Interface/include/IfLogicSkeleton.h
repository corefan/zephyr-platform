#ifndef __IFLOGIC_SKELETON_H__
#define __IFLOGIC_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfLogic.h"
#include "IfLogicMethodId.h"
namespace test_logic 
{
class IfLogicSkeleton 
{
public:
    IfLogic *m_pImplementObj;
    IfLogicSkeleton(IfLogic *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfLogicServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleTest_TUInt64_TInt32(CMessageHeader *pMsg);
};
}
#endif

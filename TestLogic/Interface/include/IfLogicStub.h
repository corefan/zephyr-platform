#ifndef __IFLOGIC_STUB_H__
#define __IFLOGIC_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfLogic.h"
namespace test_logic 
{
class IfLogicStub : public IfLogic
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 Test(TUInt64 uParm1,TInt32 n2);
};
}
#endif

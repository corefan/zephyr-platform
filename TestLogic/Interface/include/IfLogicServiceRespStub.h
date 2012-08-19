#ifndef __IFLOGICSERVICERESP_STUB_H__
#define __IFLOGICSERVICERESP_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/iflogicserviceresp.h"
namespace test_logic 
{
class IfLogicServiceRespStub : public IfLogicServiceResp
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 RespGetSession(TUInt64 uId,CDoid tGwDoid);
    virtual TInt32 RespReleaseSession(TUInt64 uId);
};
}
#endif

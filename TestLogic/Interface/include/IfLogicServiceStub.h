#ifndef __IFLOGICSERVICE_STUB_H__
#define __IFLOGICSERVICE_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfLogicService.h"
namespace test_logic 
{
class IfLogicServiceStub : public IfLogicService
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 ReqGetSession(TUInt64 uId,CDoid tGwDoid,OctSeq<TUInt16> tData);
    virtual TInt32 ReqReleaseSession(TUInt64 uId,CDoid tSess,OctSeq<TUInt16> tData);
};
}
#endif

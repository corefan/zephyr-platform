#ifndef __IFLOGICRESP_STUB_H__
#define __IFLOGICRESP_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/iflogicResp.h"
namespace test_logic 
{
class IfLogicRespStub : public IfLogicResp
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 RespTest(TInt32 nRslt);
};
}
#endif

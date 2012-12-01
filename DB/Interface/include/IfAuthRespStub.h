#ifndef __IFAUTHRESP_STUB_H__
#define __IFAUTHRESP_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfAuthResp.h"
namespace erp_platform 
{
class IfAuthRespStub : public IfAuthResp
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 RespAuthenticate(TInt32 nResult,TUInt64 uId);
    virtual TInt32 ConfirmDisconneted(CDoid tMyDoid);
};
}
#endif

#ifndef __IFGATEWAYRESP_STUB_H__
#define __IFGATEWAYRESP_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfGatewayResp.h"
namespace Zephyr 
{
class IfGatewayRespStub : public IfGatewayResp
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 ConfirmRegisterService(TUInt32 uServiceId);
    virtual TInt32 ConfirmUnRegisterService(TUInt32 uServiceId);
};
}
#endif

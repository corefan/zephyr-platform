#ifndef __IFAUTHSERVICE_STUB_H__
#define __IFAUTHSERVICE_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfAuthService.h"
namespace Zephyr 
{
class IfAuthServiceStub : public IfAuthService
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData);
};
}
#endif

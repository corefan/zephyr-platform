#ifndef __IFAUTHSERVICE_STUB_H__
#define __IFAUTHSERVICE_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfAuthService.h"
namespace erp_platform 
{
class IfAuthServiceStub : public IfAuthService
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 Authenticate(TUInt32 uDyncNr,TChar* pszName,TChar* pszPwd);
    virtual TInt32 ChangePwd(TChar* pszName,TChar* pszOldPwd,TChar* pNewPwd);
    virtual TInt32 OnDisconneted(CDoid tMyDoid);
};
}
#endif

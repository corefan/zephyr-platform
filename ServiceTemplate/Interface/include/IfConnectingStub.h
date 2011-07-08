#ifndef __IFCONNECTING_STUB_H__
#define __IFCONNECTING_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
namespace Zephyr 
{
class IfConnectingStub : public IfConnecting
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    TInt32 RegisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid* pDoid);
    TInt32 UnregisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid* pDoid);
    TInt32 Disconnect(TUInt32 uReason);
    TInt32 SetId(TUInt32 uId);
    TInt32 CheckId(TUInt32 uId);
};
}
#endif

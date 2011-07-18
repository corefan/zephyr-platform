#ifndef __IFGATEWAYSVC_STUB_H__
#define __IFGATEWAYSVC_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfGateway.h"
namespace Zephyr 
{
class IfGatewaySvcStub : public IfGatewaySvc
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    TInt32 Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV);
    TInt32 RegisterService(TUInt32 uServiceId,TUInt32 uServicBegin,TUInt32 uEnd,TUInt32 uPriority,OctSeq<TUInt16>& tServiceName);
    TInt32 UnRegisterService(TUInt32 uServiceId,TUInt32 uServicBegin,TUInt32 uEnd);
    TInt32 BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName);
    TInt32 ChangePriorty(TUInt32 uServiceId,CDoid* pMyDoid,TUInt32 uPriority);
    TInt32 StartLogin(TUInt32 uIp,TUInt16 nListeningPort,TUInt16 nMaxConnection);
    TInt32 StopLogin();
    TInt32 DisconnectedAllClient();
};
}
#endif

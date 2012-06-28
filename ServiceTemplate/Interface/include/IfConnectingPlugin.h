#ifndef __ZEPHYR_IF_CONNECTING_PLUGIN_H__
#define __ZEPHYR_IF_CONNECTING_PLUGIN_H__

#include "Public/include/TypeDef.h"
#include "Public/include/SysMacros.h"
#include "../Interface/IfConnectingResp.h"
#include "../../../Public/Interface/App/include/IfScheduler.h"
#include "../../../Public/Orb/include/DistributedObject.h"
namespace Zephyr
{

class CIfConnectingPlugin : public IfConnectingResp
                          , public IfScheduler
{
public:
    enum EnConnectionState
    {
        en_inited,     
        en_registering =  0x01,
        en_registered  =  0x02,
        en_unregistering =0x04,
        en_unregistered = 0x08,
        en_disconnecting= 0x10,
        en_disconnected = 0x20,
    };
private:
    CDoid m_tRegisteredDoid;
    TUInt32 m_uServiceId;
    TUInt32 m_uServiceBegin;
    TUInt32 m_uServiceIdEnd;
    TUInt32 m_uPrioirty;
    EnConnectionState m_enState;
    void *m_pTimer;
    CObject *m_pOwner;
public:
    CIfConnectingPlugin(CObject *pOwner)
    {
        m_pOwner = pOwner;
    }
    ~CIfConnectingPlugin()
    {
        OnFinal();
    }
    TBOOL IsConnected()
    {
        if (m_enState < en_disconnecting)
        {
            return True;
        }
        return False;
    }
    CDoid GetGwDoid()
    {
        return m_tRegisteredDoid;
    }
    virtual TInt32 OnScheduler(TUInt64 nTimeNow,void *pTimer,TInt32 nRepeatTimeRemains);
    TInt32 RegisterService(CDoid* pDoid,TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,TUInt32 uPriority);
    TInt32 UnregisterService(TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd);
    TInt32 Disconnect(TUInt32 uReason);
    void OnFinal();
};


}


#endif

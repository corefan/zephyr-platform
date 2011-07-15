#ifndef __ZEPHYR_GATEWAY_SESSION_H__
#define __ZEPHYR_GATEWAY_SESSION_H__

#include "Public/include/TypeDef.h"
#include "Public/include/SysMacros.h"
#include "Public/Orb/include/Session.h"
#include "../../Interface/Interface/IfConnecting.h"
#include "Public/include/IdlHeaderFile.h"
#include "Public/tpl/include/TplList.h"
#include "Route.h"

namespace Zephyr
{

class CGatewayService;

class CGatewaySession : public CSession
                      , public IfConnecting
{
private:
    TplMultiKeyMap<CRoute,TUInt32> m_tRouteMap;
    CGatewayService *m_pService;
public:
    DECALRE_HANDLE_INTERFCE

    CGatewaySession();
    void Init(CGatewayService *pService);

    void OnConnected(TUInt32 uIp,TUInt16 uPortId);
    
    //在初始化的时候会被调.
    virtual TInt32      OnInit();
    //结束是回调.
    virtual TInt32      OnFinal();
    virtual TInt32 RegisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid *pDoid);
    virtual TInt32 UnregisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid *pDoid);
    virtual TInt32 Disconnect(TUInt32 uReason);
    virtual TInt32 SetId(TUInt32 uId);
    virtual TInt32 CheckId(TUInt32 uId);

};



}

#endif

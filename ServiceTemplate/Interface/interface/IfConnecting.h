#ifndef __ZEPHYR_IF_CONNECTING_H__
#define __ZEPHYR_IF_CONNECTING_H__

#include "Typedef.h"

namespace Zephyr
{
    class IfConnecting
    {
    public:
        TInt32 RegisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid *pDoid) = 0;
        TInt32 UnregisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid *pDoid) = 0;
        TInt32 Disconnect(TUInt32 uReason) = 0;
        TInt32 SetId(TUInt32 uId) = 0;
        TInt32 CheckId(TUInt32 uId) = 0;
    };

}


#endif

#ifndef __ZEPHYR_IF_CONNECTING_H__
#define __ZEPHYR_IF_CONNECTING_H__

#include "Public/include/Typedef.h"

namespace Zephyr
{
    enum EnDisconnectType
    {
        en_direct_dis,                //disconnected the client directly,
        en_disconnect_with_delay,     //delay 5 sec for the client 2 recv all the data.
        en_disconnect_without_reply,  //disconnect the client directly and need no info to the register.
    };
    class IfConnecting
    {
    public:
        virtual TInt32 RegisterService(CDoid *pDoid,TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,TUInt32 uPriority) = 0;
        virtual TInt32 UnregisterService(TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd) = 0;
        //×¢²á¹ã²¥
        virtual TInt32 RegisterTeam(TUInt32 uTeamID) = 0;
        //¶ÏÏß
        virtual TInt32 Disconnect(TUInt32 uReason) = 0;
        virtual TInt32 SetId(TUInt32 uId) = 0;
        virtual TInt32 CheckId(TUInt32 uId) = 0;

        virtual TInt32 SendCryptedKey(OctSeq<TUInt16> tKey) = 0;
    };
}


#endif

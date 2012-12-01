#ifndef __ZEPHYR_IF_CONNECTING_RESP_H__
#define __ZEPHYR_IF_CONNECTING_RESP_H__

#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
namespace Zephyr
{



class IfConnectingResp
{
public:
    virtual TInt32 RespRegisterService(CDoid *pDoid,TUInt32 uServiceId,TInt32 nRslt) = 0;
    virtual TInt32 ConfirmUnregisterService(TUInt32 uServiceId) = 0;
    //×¢²á¹ã²¥

    virtual TInt32 OnDisconnect(TUInt32 uReason) = 0;

    virtual TInt32 SendRSAPublicKey(TUInt16 uBits,TUInt16 isFermat4,OctSeq<TUInt16> e,OctSeq<TUInt16> n,TUInt32 uDyncNr) = 0;
};

}

#endif
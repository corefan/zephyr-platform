#ifndef __ZEPHYR_DB_IF_AUTH_SERVICE_H__
#define __ZEPHYR_DB_IF_AUTH_SERVICE_H__

#include "Public/include/TypeDef.h"

namespace Zephyr
{

class IfAuthService
{
public:
    virtual TInt32 Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData) = 0;
    virtual TInt32 OnDisconneted(CDoid tMyDoid) = 0;
};



}

#endif

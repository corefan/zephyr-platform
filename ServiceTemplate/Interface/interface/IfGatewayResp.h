#ifndef __ZEPHYR_IF_GATEWAY_RESP_H__
#define __ZEPHYR_IF_GATEWAY_RESP_H__

#include "Public/include/TypeDef.h"

namespace Zephyr
{

class IfGatewayResp
{
public:
    virtual TInt32 ConfirmRegisterService(TUInt32 uServiceId) = 0;
    //×¢Ïú·þÎñ
    virtual TInt32 ConfirmUnRegisterService(TUInt32 uServiceId) = 0;
};

}


#endif

#ifndef __TEST_LOGIC_IF_LOGIC_SERVICE_RESP_H__
#define __TEST_LOGIC_IF_LOGIC_SERVICE_RESP_H__
#include "PUblic/include/TypeDef.h"

namespace test_logic
{

class IfLogicServiceResp
{
public:
    virtual TInt32 RespGetSession(TUInt64 uId,CDoid tGwDoid) = 0;
    virtual TInt32 RespReleaseSession(TUInt64 uId) = 0;
};
}

#endif

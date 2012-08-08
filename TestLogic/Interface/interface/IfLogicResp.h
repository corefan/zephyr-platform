#ifndef __TEST_LOGIC_IF_LOGIC_RESP_H__
#define __TEST_LOGIC_IF_LOGIC_RESP_H__
#include "Public/include/TypeDef.h"

namespace test_logic
{
class IfLogicResp
{
public:
    virtual TInt32 RespTest(TInt32 nRslt) = 0;
};
}

#endif

#ifndef __TEST_LOGIC_IF_LOGIC_H__
#define __TEST_LOGIC_IF_LOGIC_H__
#include "Public/include/TypeDef.h"

namespace test_logic
{

class IfLogic
{
public:
    virtual TInt32 Test(TUInt64 uParm1,TInt32 n2) = 0;
};

}
#endif

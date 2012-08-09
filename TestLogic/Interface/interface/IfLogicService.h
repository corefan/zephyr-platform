#ifndef __TEST_LOGIC_IF_LOGIC_SERVICE_H__
#define __TEST_LOGIC_IF_LOGIC_SERVICE_H__

#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
namespace test_logic
{

class IfLogicService
{
public:
    virtual TInt32 ReqGetSession(TUInt64 uId,CDoid tGwDoid,OctSeq<TUInt16> tData) = 0;
    virtual TInt32 ReqReleaseSession(TUInt64 uId,CDoid tSess,OctSeq<TUInt16> tData) = 0;
};

}

#endif

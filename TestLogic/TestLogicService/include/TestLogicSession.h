#ifndef __TEST_LOGIC_TEST_LOGIC_SESSION_H__
#define __TEST_LOGIC_TEST_LOGIC_SESSION_H__
#include "Public/include/TypeDef.h"
#include "Public/include/SysMacros.h"
#include "Public/Orb/include/Session.h"
#include "../../Interface/interface/IfLogic.h"
#include "../../../ServiceTemplate/Interface/include/IfConnectingPlugin.h"
using namespace Zephyr;

namespace test_logic
{

class CTestLogicSession : public CSession
                        , public IfLogic
{
public:
    CIfConnectingPlugin m_tClient;
public :
    CTestLogicSession();
    DECALRE_HANDLE_INTERFCE;

    TInt32 Init(CDoid tGw);
    virtual TInt32 OnInited();
    virtual void OnFinal();
    virtual TInt32 Test(TUInt64 uParm1,TInt32 n2);
};

}
#endif

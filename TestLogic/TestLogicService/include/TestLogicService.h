#ifndef __TEST_LOGIC_TEST_LOGIC_SERVICE_H__
#define __TEST_LOGIC_TEST_LOGIC_SERVICE_H__
#include "Public/include/TypeDef.h"
#include "../../Interface/interface/IfLogicService.h"
#include "Public/Orb/include/Service.h"
#include "Public/tpl/include/TplPool.h"
#include "TestLogicLoggerDef.h"
#include "TestLogicSession.h"
#include <map>
using namespace Zephyr;
namespace test_logic
{
class CTestLogic : public CService 
                 , public IfLogicService
{
private:
    CPool<CTestLogicSession> m_tPools;
    std::map<TUInt64,CTestLogicSession*> m_tRoleMap;
public:
    DECALRE_HANDLE_INTERFCE;
    CTestLogic();
    virtual TInt32 OnInit();
    virtual void OnFinal();
    virtual TInt32 OnRoutine(TUInt32 nRunCnt);
    virtual TInt32 ReqGetSession(TUInt64 uId,CDoid tGwDoid,OctSeq<TUInt16> tData);
    virtual TInt32 ReqReleaseSession(TUInt64 uId,CDoid tSess);

private:
    CTestLogicSession *GetSession(TUInt64 uId);
    void ReleaseSession(CTestLogicSession *pSess);
};

}

#endif

#include "../include/TestLogicService.h"
#include "../../Interface/include/IfLogicServiceSkeleton.h"
namespace test_logic
{

IMPLEMENT_START_HANDLE_INTERFACE(CTestLogic)
IMPLEMENT_HANDLE_INTERFACE(IfLogicService)
IMPLEMENT_END_HANDLE_INTERFACE(CTestLogic)

CTestLogic::CTestLogic()
{

}

TInt32 CTestLogic::OnInit()
{
    return SUCCESS;
}
void CTestLogic::OnFinal()
{

}

TInt32 CTestLogic::OnRoutine(TUInt32 nRunCnt)
{
    return SUCCESS;
}

TInt32 CTestLogic::ReqGetSession(TUInt64 uId,CDoid tGwDoid,OctSeq<TUInt16> tData)
{
    return SUCCESS;
}

TInt32 CTestLogic::ReqReleaseSession(TUInt64 uId,CDoid tSess)
{
    return SUCCESS;
}

CTestLogicSession *CTestLogic::GetSession(TUInt64 uId)
{
    CTestLogicSession *pSession = m_tPools.GetMem();
    if (pSession)
    {
        pSession->CSession::Init((CService*)this);
    }
    return pSession;
}

void CTestLogic::ReleaseSession(CTestLogicSession *pSess)
{
    return ;
}
}
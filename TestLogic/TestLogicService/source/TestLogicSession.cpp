#include "../include/TestLogicSession.h"
#include "../../Interface/include/IfLogicSkeleton.h"
namespace test_logic
{
IMPLEMENT_START_HANDLE_INTERFACE(CTestLogicSession)
IMPLEMENT_HANDLE_INTERFACE(IfLogic)
IMPLEMENT_END_HANDLE_INTERFACE(CTestLogicSession)

CTestLogicSession::CTestLogicSession():m_tClient(this)
{

}

TInt32 CTestLogicSession::Init(CDoid tGw)
{
    m_tClient.Init(NULL,&tGw);
    //m_tClient.RegisterService(GetMyDoid(),IfLogicServiceId,(IfLogicServiceIdBegin),(IfLogicServiceIdEnd),0);
    return REGISTER_INTERFACE_2_GW(m_tClient,IfLogic);
}

TInt32 CTestLogicSession::OnInited()
{
    return SUCCESS;
}

void CTestLogicSession::OnFinal()
{

}

TInt32 CTestLogicSession::Test(TUInt64 uParm1,TInt32 n2)
{
    return SUCCESS;
}

}
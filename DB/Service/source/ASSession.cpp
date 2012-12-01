#include "../include/ASSession.h"
#include "../../Interface/include/IfUserSessionSkeleton.h"
namespace erp_platform
{
CASSession::CASSession()
{
}
CASSession::~CASSession()
{
}

IMPLEMENT_START_HANDLE_INTERFACE(CASSession)
IMPLEMENT_HANDLE_INTERFACE(IfUserSession)
IMPLEMENT_END_HANDLE_INTERFACE(CASSession)

void CASSession::OnInit(CAuthenticateService *pService,CAuthorityData *pData,CDoid tGwDoid)
{
    m_pService = pService;
    memcpy(&m_tAuthorityData,pData,sizeof(CAuthorityData));
    m_tGwDoid = tGwDoid;
}

TInt32 CASSession::OnInited()
{
    return SUCCESS;
}
    //��ʼ�ͷ�Session
void CASSession::StartRelease()
{
    
}
    //��ʼ����session
void CASSession::CreateAllSessions()
{

}


void CASSession::OnFinal()
{
}

    //to check if all the session is created��
TBOOL CASSession::IsAllSessionCreated()
{
    return True;
}
    //check if all the session is released.
TBOOL CASSession::IsAllSessionReleased()
{
    return False;
}

//��������
TInt32 CASSession::ChangePwd(TChar *pOldPwd,TChar *pszNewPwd)
{
    return SUCCESS;
}
//��ȡ������Ϣ,nVersionNr �Ǳ��ش���������ʱ�䣬�������û����Ϊ0
TInt32 CASSession::ReqGetMyInfo(TUInt32 uLastUpdateTime)
{
    return SUCCESS;
}

//��ȡ���˵���Ϣ
TInt32 CASSession::ReqGetUserInfo(TUInt32 uUserId)
{
    return SUCCESS;
}
//��ȡ������Ϣ
TInt32 CASSession::ReqGetDepartmentInfo(TUInt32 uDepartmentId)
{
    return SUCCESS;
}

TInt32 CASSession::ReqGetMenuInfoList(TUInt32 uFatherId)
{
    return SUCCESS;
}

//����ĳһ������
TInt32 CASSession::ReqUpdateMenuInfo(TJobInfos &rJobInfos)
{
    return SUCCESS;
}

TInt32 CASSession::ReqGetActiveWorkInfo(TInt32 nJobId,TUInt32 uRcdId)
{
    return SUCCESS;
}

//��һ��ʱ���ڵ�����
TInt32 CASSession::ReqGetMyWorkList(TUInt32 uFromTime,TUInt32 uToTime)
{
    return SUCCESS;
}


TInt32 CASSession::ReqGetMenuInfoById(TUInt32 uMenuId)
{
    return SUCCESS;
}


}
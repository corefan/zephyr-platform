#ifndef __ZEPHYR_AS_SESSION_H__
#define __ZEPHYR_AS_SESSION_H__

#include "Public/include/TypeDef.h"
#include "Public/Orb/include/Session.h"
#include "AuthenticateData.h"
#include "../../../TestLogic/Interface/include/IfLogicServicePlugin.h"
#include "../../Interface/Interface/IfUserSession.h"
#include <set>
using namespace test_logic;

namespace erp_platform
{

class CAuthenticateService;

//ֻ�лص�.���ڸ��ٸ�����Դ��ʹ��״��
class CASSession : public CSession
                 , public IfUserSession
{
private:
    CDoid m_tGwDoid; 
    CAuthorityData m_tAuthorityData;
    TUInt32 m_uStartInitTime; //��ʼ��ʼ����ʱ��.
    CAuthenticateService *m_pService; //����
    IfLogicServicePlugin m_tLogicServicePlugin;
    set<TInt32>   m_tRights;
public:
    enum EnState
    {
        en_just_created = 0,
        en_inited,
        //���ڴ�����̨Session
        en_creating_session,
        //���к�̨session������ϣ����ڷ�����
        en_all_session_created,
        //��ʼ�ͷţ����ʱ��m_tGwDoid����ΪNULL_DOID;
        en_start_releasing,
        //�����ͷŵ��ˣ���ô�ͻ����ڴ�.
        en_all_released,
    };
    CASSession();
    ~CASSession();
    DECALRE_HANDLE_INTERFCE;
    void OnInit(CAuthenticateService *pService,CAuthorityData *pData,CDoid tGwDoid);
    virtual TInt32 OnInited();
    //��ʼ�ͷ�Session
    void StartRelease();
    //��ʼ����session
    void CreateAllSessions();

    virtual TInt32 ChangePwd(TChar *pOldPwd,TChar *pszNewPwd);
    //��ȡ������Ϣ,nVersionNr �Ǳ��ش���������ʱ�䣬�������û����Ϊ0
    virtual TInt32 ReqGetMyInfo(TUInt32 uLastUpdateTime);
    //��ȡ���˵���Ϣ
    virtual TInt32 ReqGetUserInfo(TUInt32 uUserId);
    //��ȡ������Ϣ
    virtual TInt32 ReqGetDepartmentInfo(TUInt32 uDepartmentId);

    virtual TInt32 ReqGetMenuInfoList(TUInt32 uFatherId);
    //����ĳһ������
    virtual TInt32 ReqUpdateMenuInfo(TJobInfos &rJobInfos);

    virtual TInt32 ReqGetActiveWorkInfo(TInt32 nJobId,TUInt32 uRcdId);
    //��һ��ʱ���ڵ�����
    virtual TInt32 ReqGetMyWorkList(TUInt32 uFromTime,TUInt32 uToTime);

    virtual TInt32 ReqGetMenuInfoById(TUInt32 uMenuId);

    virtual void OnFinal();

private:
    //to check if all the session is created��
    TBOOL IsAllSessionCreated();
    //check if all the session is released.
    TBOOL IsAllSessionReleased();
};


}


#endif

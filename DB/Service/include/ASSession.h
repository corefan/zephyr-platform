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

//只有回调.用于跟踪各个资源的使用状况
class CASSession : public CSession
                 , public IfUserSession
{
private:
    CDoid m_tGwDoid; 
    CAuthorityData m_tAuthorityData;
    TUInt32 m_uStartInitTime; //开始初始化的时间.
    CAuthenticateService *m_pService; //服务
    IfLogicServicePlugin m_tLogicServicePlugin;
    set<TInt32>   m_tRights;
public:
    enum EnState
    {
        en_just_created = 0,
        en_inited,
        //正在创建后台Session
        en_creating_session,
        //所有后台session创建完毕，正在服务中
        en_all_session_created,
        //开始释放，这个时候m_tGwDoid被置为NULL_DOID;
        en_start_releasing,
        //所有释放掉了，那么就回收内存.
        en_all_released,
    };
    CASSession();
    ~CASSession();
    DECALRE_HANDLE_INTERFCE;
    void OnInit(CAuthenticateService *pService,CAuthorityData *pData,CDoid tGwDoid);
    virtual TInt32 OnInited();
    //开始释放Session
    void StartRelease();
    //开始创建session
    void CreateAllSessions();

    virtual TInt32 ChangePwd(TChar *pOldPwd,TChar *pszNewPwd);
    //获取本人信息,nVersionNr 是本地存的最近更新时间，如果本地没有则为0
    virtual TInt32 ReqGetMyInfo(TUInt32 uLastUpdateTime);
    //获取别人的信息
    virtual TInt32 ReqGetUserInfo(TUInt32 uUserId);
    //获取部门信息
    virtual TInt32 ReqGetDepartmentInfo(TUInt32 uDepartmentId);

    virtual TInt32 ReqGetMenuInfoList(TUInt32 uFatherId);
    //更新某一个配置
    virtual TInt32 ReqUpdateMenuInfo(TJobInfos &rJobInfos);

    virtual TInt32 ReqGetActiveWorkInfo(TInt32 nJobId,TUInt32 uRcdId);
    //我一段时间内的任务
    virtual TInt32 ReqGetMyWorkList(TUInt32 uFromTime,TUInt32 uToTime);

    virtual TInt32 ReqGetMenuInfoById(TUInt32 uMenuId);

    virtual void OnFinal();

private:
    //to check if all the session is created！
    TBOOL IsAllSessionCreated();
    //check if all the session is released.
    TBOOL IsAllSessionReleased();
};


}


#endif

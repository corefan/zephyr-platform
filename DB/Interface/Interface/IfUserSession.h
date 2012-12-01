#ifndef __ZEPHYR_IF_USER_SESSION_H__
#define __ZEPHYR_IF_USER_SESSION_H__

#include "Public/include/TypeDef.h"
#include "JobInfo.h"
namespace erp_platform
{

class IfUserSession 
{
public:
    //更改密码
    virtual TInt32 ChangePwd(TChar *pOldPwd,TChar *pszNewPwd) = 0;
    //获取本人信息,nVersionNr 是本地存的最近更新时间，如果本地没有则为0
    virtual TInt32 ReqGetMyInfo(TUInt32 uLastUpdateTime) = 0;
    //获取别人的信息
    virtual TInt32 ReqGetUserInfo(TUInt32 uUserId) = 0;
    //获取部门信息
    virtual TInt32 ReqGetDepartmentInfo(TUInt32 uDepartmentId) = 0;

    virtual TInt32 ReqGetMenuInfoList(TUInt32 uFatherId) = 0;
    //更新某一个配置
    virtual TInt32 ReqUpdateMenuInfo(TJobInfos &rJobInfos) = 0;

    virtual TInt32 ReqGetActiveWorkInfo(TInt32 nJobId,TUInt32 uRcdId) = 0;
    //我一段时间内的任务
    virtual TInt32 ReqGetMyWorkList(TUInt32 uFromTime,TUInt32 uToTime) = 0;

    virtual TInt32 ReqGetMenuInfoById(TUInt32 uMenuId) = 0;
};

}



#endif

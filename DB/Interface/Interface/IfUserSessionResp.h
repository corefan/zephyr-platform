#ifndef __ZEPHYR_IF_USER_SESSION_RESP_H__
#define __ZEPHYR_IF_USER_SESSION_RESP_H__

#include "Public/include/TypeDef.h"
#include <map>
#include <list>
#include "JobInfo.h"
using namespace std;
using namespace Zephyr;

namespace erp_platform
{
    
// 
//     struct TJobList
//     {
//         list<TJobInfos> m_tJobLists;
//     };

class IfUsersessionResp
{
public:
    virtual TInt32 OnPwdChanged(TInt32 nResult) = 0;
    //UpdateTime最近更新时间,pszName名字,pszApartName,部门名字,uMyId,我的数字ID,uApartId,部门ID
    virtual TInt32 RespGetMyInfo(TUInt32 uUpdateTime,TChar *pszName,TChar *pszApartName,TUInt32 uMyId,TUInt32 uApartId) = 0;
    //返回别人的信息,这个东西应该是经常调用的.
    virtual TInt32 RespGetUserInfo(TUInt32 uUpdateTime,TChar *pszName,TChar *pszApartName,TUInt32 uMyId,TUInt32 uApartId) = 0;
    //返回部门信息
    virtual TInt32 RespGetDepartInfo(TUInt32 uUpdateTime,TChar *pszApartName,TUInt32 uApartId) = 0;

    //返回的是功能列表，这个只有登录的时候会发送一次，并且
    virtual TInt32 SendJobList(list<TJobInfos> &tMenu2,list<TJobInfos> &tMeun3) = 0;

    //发送当前任务,tJobs是所属的工作流程，uRcdId,是这个工作对应的数据库记录的ID，pszTitle是主题，这个会发多次，并且，会实时的推送
    virtual TInt32 SendActiveWork(TJobInfos tJobs,TUInt32 uRcdId,const TChar *pszTitle) = 0;
    
    //他们的fatherId都是相同的，即请求的id
    virtual TInt32 RespMenuList(list<TJobInfos> tList) = 0;

    //获取特定的流程名
    virtual TInt32 RespGetMenuInfo(TJobInfos &tJobInfo) = 0;
};

}

#endif

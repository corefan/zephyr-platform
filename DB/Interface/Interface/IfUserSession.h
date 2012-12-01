#ifndef __ZEPHYR_IF_USER_SESSION_H__
#define __ZEPHYR_IF_USER_SESSION_H__

#include "Public/include/TypeDef.h"
#include "JobInfo.h"
namespace erp_platform
{

class IfUserSession 
{
public:
    //��������
    virtual TInt32 ChangePwd(TChar *pOldPwd,TChar *pszNewPwd) = 0;
    //��ȡ������Ϣ,nVersionNr �Ǳ��ش���������ʱ�䣬�������û����Ϊ0
    virtual TInt32 ReqGetMyInfo(TUInt32 uLastUpdateTime) = 0;
    //��ȡ���˵���Ϣ
    virtual TInt32 ReqGetUserInfo(TUInt32 uUserId) = 0;
    //��ȡ������Ϣ
    virtual TInt32 ReqGetDepartmentInfo(TUInt32 uDepartmentId) = 0;

    virtual TInt32 ReqGetMenuInfoList(TUInt32 uFatherId) = 0;
    //����ĳһ������
    virtual TInt32 ReqUpdateMenuInfo(TJobInfos &rJobInfos) = 0;

    virtual TInt32 ReqGetActiveWorkInfo(TInt32 nJobId,TUInt32 uRcdId) = 0;
    //��һ��ʱ���ڵ�����
    virtual TInt32 ReqGetMyWorkList(TUInt32 uFromTime,TUInt32 uToTime) = 0;

    virtual TInt32 ReqGetMenuInfoById(TUInt32 uMenuId) = 0;
};

}



#endif

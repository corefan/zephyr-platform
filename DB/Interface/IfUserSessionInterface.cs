using System.Collections;
using System.Collections.Generic;
using System;
public interface IfUserSession
{
    int ChangePwd(char _pOldPwd,char _pszNewPwd);
    int ReqGetMyInfo(uint _uLastUpdateTime);
    int ReqGetUserInfo(uint _uUserId);
    int ReqGetDepartmentInfo(uint _uDepartmentId);
    int ReqGetMenuInfoList(uint _uFatherId);
    int ReqUpdateMenuInfo(TJobInfos _rJobInfos);
    int ReqGetActiveWorkInfo(int _nJobId,uint _uRcdId);
    int ReqGetMyWorkList(uint _uFromTime,uint _uToTime);
    int ReqGetMenuInfoById(uint _uMenuId);
}

public enum IfUserSessionMethodId
{
    IfUserSessionMethodIdBegin=1400,
    IfUserSessionChangePwdMethodId = IfUserSessionMethodIdBegin,
    IfUserSessionReqGetMyInfoMethodId,
    IfUserSessionReqGetUserInfoMethodId,
    IfUserSessionReqGetDepartmentInfoMethodId,
    IfUserSessionReqGetMenuInfoListMethodId,
    IfUserSessionReqUpdateMenuInfoMethodId,
    IfUserSessionReqGetActiveWorkInfoMethodId,
    IfUserSessionReqGetMyWorkListMethodId,
    IfUserSessionReqGetMenuInfoByIdMethodId,
    IfUserSessionMethodIdEnd,
}

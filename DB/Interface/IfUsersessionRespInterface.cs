using System.Collections;
using System.Collections.Generic;
using System;
public interface IfUsersessionResp
{
    int OnPwdChanged(int _nResult);
    int RespGetMyInfo(uint _uUpdateTime,char _pszName,char _pszApartName,uint _uMyId,uint _uApartId);
    int RespGetUserInfo(uint _uUpdateTime,char _pszName,char _pszApartName,uint _uMyId,uint _uApartId);
    int RespGetDepartInfo(uint _uUpdateTime,char _pszApartName,uint _uApartId);
    int SendJobList(list<TJobInfos> _tMenu2,list<TJobInfos> _tMeun3);
    int SendActiveWork(TJobInfos _tJobs,uint _uRcdId,char _pszTitle);
    int RespMenuList(list<TJobInfos> _tList);
    int RespGetMenuInfo(TJobInfos _tJobInfo);
}

public enum IfUsersessionRespMethodId
{
    IfUsersessionRespMethodIdBegin=1600,
    IfUsersessionRespOnPwdChangedMethodId = IfUsersessionRespMethodIdBegin,
    IfUsersessionRespRespGetMyInfoMethodId,
    IfUsersessionRespRespGetUserInfoMethodId,
    IfUsersessionRespRespGetDepartInfoMethodId,
    IfUsersessionRespSendJobListMethodId,
    IfUsersessionRespSendActiveWorkMethodId,
    IfUsersessionRespRespMenuListMethodId,
    IfUsersessionRespRespGetMenuInfoMethodId,
    IfUsersessionRespMethodIdEnd,
}

using System.Collections;
using System.Collections.Generic;
using System;
public interface IfAuthService
{
    int Authenticate(uint _uDyncNr,char _pszName,char _pszPwd);
    int ChangePwd(char _pszName,char _pszOldPwd,char _pNewPwd);
    int OnDisconneted(CDoid _tMyDoid);
}

public enum IfAuthServiceMethodId
{
    IfAuthServiceMethodIdBegin=1000,
    IfAuthServiceAuthenticateMethodId = IfAuthServiceMethodIdBegin,
    IfAuthServiceChangePwdMethodId,
    IfAuthServiceOnDisconnetedMethodId,
    IfAuthServiceMethodIdEnd,
}

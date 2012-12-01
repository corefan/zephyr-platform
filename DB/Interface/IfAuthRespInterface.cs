using System.Collections;
using System.Collections.Generic;
using System;
public interface IfAuthResp
{
    int RespAuthenticate(int _nResult,ulong _uId);
    int ConfirmDisconneted(CDoid _tMyDoid);
}

public enum IfAuthRespMethodId
{
    IfAuthRespMethodIdBegin=1200,
    IfAuthRespRespAuthenticateMethodId = IfAuthRespMethodIdBegin,
    IfAuthRespConfirmDisconnetedMethodId,
    IfAuthRespMethodIdEnd,
}

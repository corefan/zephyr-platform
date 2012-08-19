using System.Collections;
using System.Collections.Generic;
using System;
public interface IfLogicResp
{
    int RespTest(int _nRslt);
}

public enum IfLogicRespMethodId
{
    IfLogicRespMethodIdBegin=4800,
    IfLogicRespRespTestMethodId = IfLogicRespMethodIdBegin,
    IfLogicRespMethodIdEnd,
}

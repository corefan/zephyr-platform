using System.Collections;
using System.Collections.Generic;
using System;
interface IfLogicResp
{
    int RespTest(int _nRslt);
}

enum IfLogicRespMethodId
{
    IfLogicRespMethodIdBegin=3800,
    IfLogicRespRespTestMethodId,
    IfLogicRespMethodIdEnd,
}

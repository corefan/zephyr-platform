using System.Collections;
using System.Collections.Generic;
using System;
interface IfLogic
{
    int Test(ulong _uParm1,int _n2);
}

enum IfLogicMethodId
{
    IfLogicMethodIdBegin=3600,
    IfLogicTestMethodId,
    IfLogicMethodIdEnd,
}

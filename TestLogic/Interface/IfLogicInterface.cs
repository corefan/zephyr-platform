using System.Collections;
using System.Collections.Generic;
using System;
public interface IfLogic
{
    int Test(ulong _uParm1,int _n2);
}

public enum IfLogicMethodId
{
    IfLogicMethodIdBegin=4600,
    IfLogicTestMethodId = IfLogicMethodIdBegin,
    IfLogicMethodIdEnd,
}

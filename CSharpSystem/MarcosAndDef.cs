using UnityEngine;
using System.Collections;

public class MacrosAndDef
{
    public const int SUCCESS = 0;
    public const int FAIL = -1;
    public const int NOT_ENOUGH_BUFFER = -2;
    public const int OUT_OF_MEM = -3;
    public const int MSG_HEADER_LEN = (sizeof(uint)+sizeof(int));
    public const int MSG_NOT_HANDLED = -4;
}

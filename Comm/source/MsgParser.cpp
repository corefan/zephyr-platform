#include "MsgParser.h"

namespace Zephyr
{


TInt32  CMsgParser::OnRecv(TUChar *pNetData,TInt32 dataLen)
{
    CMessageHeader *pMsg = (CMessageHeader*)pNetData;
    TInt32 msgLen = pMsg->GetLength();
    TInt32 returnLen = 0; 
    while (msgLen <= dataLen)
    {
        dataLen -= msgLen;
        returnLen += msgLen;
        pMsg = (CMessageHeader*)(pNetData + returnLen);
        msgLen = pMsg->GetLength();
    }
    return returnLen;
}

}//end of namespace Zephyr;
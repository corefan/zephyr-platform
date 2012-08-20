#include "../include/IfLogicSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfLogicMethodId.h"
namespace test_logic 
{
TInt32 IfLogicSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfLogicSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {TEST_TUINT64_TINT32_ID, &IfLogicSkeleton::HandleTest_TUInt64_TInt32},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 1;
    TUInt32 nMethodId = pMsg->GetMethodId();
    _PFMSG pPfMsg = NULL;
    while(nBegin < nEnd)
    {
        if (nBegin == (nEnd -1))
        {
            if (sMsgMapEntries[nBegin].m_uMsgID == nMethodId)
            {
                pPfMsg=sMsgMapEntries[nBegin].m_pHandlerFunc;
            }
            else if (sMsgMapEntries[nEnd].m_uMsgID == nMethodId)
            {
                pPfMsg=sMsgMapEntries[nEnd].m_pHandlerFunc;
            }
            break;
        }
        TInt32 nMiddle = (nBegin + nEnd) >> 1;
        TUInt32 nMiddleVal = sMsgMapEntries[nMiddle].m_uMsgID;
        if (nMiddleVal == nMethodId)
        {
            pPfMsg = sMsgMapEntries[nMiddle].m_pHandlerFunc;
            break;
        }
        else
        {
            if (nMiddleVal>nMethodId)
            {
                nEnd = nMiddle;
            }
            else
            {
                nBegin   = nMiddle;
            }
        }
    }
    if (NULL == pPfMsg)
    {
        return CAN_NOT_HANDLE_THIS_MSG;
    }
    return (this->*pPfMsg)(pMsg);
}; 
TInt32 IfLogicSkeleton::HandleTest_TUInt64_TInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt64 _uParm1;
    nRet = Unmarshall(pBuffer,nLen,_uParm1);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TInt32 _n2;
    nRet = Unmarshall(pBuffer,nLen,_n2);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->Test(_uParm1,_n2);
    return SUCCESS;
}
}

#include "../include/IfAuthRespSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfAuthRespMethodId.h"
namespace Zephyr 
{
TInt32 IfAuthRespSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfAuthRespSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {RESPAUTHENTICATE_TINT32_ID, &IfAuthRespSkeleton::HandleRespAuthenticate_TInt32},
        {CONFIRMDISCONNETED_CDOID_ID, &IfAuthRespSkeleton::HandleConfirmDisconneted_CDoid},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 2;
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
TInt32 IfAuthRespSkeleton::HandleRespAuthenticate_TInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TInt32 _nResult;
    nRet = Unmarshall(pBuffer,nLen,_nResult);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespAuthenticate(_nResult);
    return SUCCESS;
}
TInt32 IfAuthRespSkeleton::HandleConfirmDisconneted_CDoid(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    CDoid _tMyDoid;
    nRet = Unmarshall(pBuffer,nLen,_tMyDoid);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ConfirmDisconneted(_tMyDoid);
    return SUCCESS;
}
}

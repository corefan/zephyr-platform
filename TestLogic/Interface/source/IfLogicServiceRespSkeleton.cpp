#include "../include/IfLogicServiceRespSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfLogicServiceRespMethodId.h"
namespace test_logic 
{
TInt32 IfLogicServiceRespSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfLogicServiceRespSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {RESPGETSESSION_TUINT64_CDOID_ID, &IfLogicServiceRespSkeleton::HandleRespGetSession_TUInt64_CDoid},
        {RESPRELEASESESSION_TUINT64_ID, &IfLogicServiceRespSkeleton::HandleRespReleaseSession_TUInt64},
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
TInt32 IfLogicServiceRespSkeleton::HandleRespGetSession_TUInt64_CDoid(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt64 _uId;
    nRet = Unmarshall(pBuffer,nLen,_uId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    CDoid _tGwDoid;
    nRet = Unmarshall(pBuffer,nLen,_tGwDoid);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespGetSession(_uId,_tGwDoid);
    return SUCCESS;
}
TInt32 IfLogicServiceRespSkeleton::HandleRespReleaseSession_TUInt64(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt64 _uId;
    nRet = Unmarshall(pBuffer,nLen,_uId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespReleaseSession(_uId);
    return SUCCESS;
}
}

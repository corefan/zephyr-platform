#include "../include/IfLogicServiceSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfLogicServiceMethodId.h"
namespace test_logic 
{
TInt32 IfLogicServiceSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfLogicServiceSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {IFLOGICSERVICE_INTERFACE_ID, &IfLogicServiceSkeleton::HandleReqGetSession_TUInt64_CDoid_OctSeq_tpl_begin_TUInt16_tpl_end_},
        {IFLOGICSERVICE_INTERFACE_ID, &IfLogicServiceSkeleton::HandleReqReleaseSession_TUInt64_CDoid_OctSeq_tpl_begin_TUInt16_tpl_end_},
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
TInt32 IfLogicServiceSkeleton::HandleReqGetSession_TUInt64_CDoid_OctSeq_tpl_begin_TUInt16_tpl_end_(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt64 _uId;
    nRet = Unmarshall(pBuffer,nLen,_uId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    CDoid _tGwDoid;
    nRet = Unmarshall(pBuffer,nLen,_tGwDoid);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    OctSeq<TUInt16> _tData;
    nRet = Unmarshall(pBuffer,nLen,_tData);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->ReqGetSession(_uId,_tGwDoid,_tData);
    return SUCCESS;
}
TInt32 IfLogicServiceSkeleton::HandleReqReleaseSession_TUInt64_CDoid_OctSeq_tpl_begin_TUInt16_tpl_end_(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt64 _uId;
    nRet = Unmarshall(pBuffer,nLen,_uId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    CDoid _tSess;
    nRet = Unmarshall(pBuffer,nLen,_tSess);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    OctSeq<TUInt16> _tData;
    nRet = Unmarshall(pBuffer,nLen,_tData);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->ReqReleaseSession(_uId,_tSess,_tData);
    return SUCCESS;
}
}

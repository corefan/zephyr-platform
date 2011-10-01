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
        {(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHRESP_INTERFACE_ID|RESPAUTHENTICATE_TINT32_TLV_TPL_BEGIN_TUINT16_AND_TUINT16_TPL_END__ID), &IfAuthRespSkeleton::HandleRespAuthenticate_TInt32_TLV_tpl_begin_TUInt16_and_TUInt16_tpl_end_},
        {(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHRESP_INTERFACE_ID|CONFIRMDISCONNETED_CDOID_ID), &IfAuthRespSkeleton::HandleConfirmDisconneted_CDoid},
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
TInt32 IfAuthRespSkeleton::HandleRespAuthenticate_TInt32_TLV_tpl_begin_TUInt16_and_TUInt16_tpl_end_(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TInt32 nResult;
    nRet = Unmarshall(pBuffer,nLen,nResult);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TLV<TUInt16,TUInt16> tAuthorityData;
    nRet = Unmarshall(pBuffer,nLen,tAuthorityData);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->RespAuthenticate(nResult,tAuthorityData);
    return SUCCESS;
}
TInt32 IfAuthRespSkeleton::HandleConfirmDisconneted_CDoid(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    CDoid tMyDoid;
    nRet = Unmarshall(pBuffer,nLen,tMyDoid);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->ConfirmDisconneted(tMyDoid);
    return SUCCESS;
}
}

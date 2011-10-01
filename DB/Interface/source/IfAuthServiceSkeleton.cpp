#include "../include/IfAuthServiceSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfAuthServiceMethodId.h"
namespace Zephyr 
{
TInt32 IfAuthServiceSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfAuthServiceSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHSERVICE_INTERFACE_ID|AUTHENTICATE_TLV_TPL_BEGIN_TUINT16_AND_TUINT16_TPL_END__ID), &IfAuthServiceSkeleton::HandleAuthenticate_TLV_tpl_begin_TUInt16_and_TUInt16_tpl_end_},
        {(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHSERVICE_INTERFACE_ID|ONDISCONNETED_CDOID_ID), &IfAuthServiceSkeleton::HandleOnDisconneted_CDoid},
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
TInt32 IfAuthServiceSkeleton::HandleAuthenticate_TLV_tpl_begin_TUInt16_and_TUInt16_tpl_end_(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TLV<TUInt16,TUInt16> tAuthenticateData;
    nRet = Unmarshall(pBuffer,nLen,tAuthenticateData);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->Authenticate(tAuthenticateData);
    return SUCCESS;
}
TInt32 IfAuthServiceSkeleton::HandleOnDisconneted_CDoid(CMessageHeader *pMsg)
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
    m_pImplementObj->OnDisconneted(tMyDoid);
    return SUCCESS;
}
}

#include "../include/IfConnectingRespSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfConnectingRespMethodId.h"
namespace Zephyr 
{
TInt32 IfConnectingRespSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfConnectingRespSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {RESPREGISTERSERVICE_CDOID_PT_TUINT32_TINT32_ID, &IfConnectingRespSkeleton::HandleRespRegisterService_CDoid_pt_TUInt32_TInt32},
        {CONFIRMUNREGISTERSERVICE_TUINT32_ID, &IfConnectingRespSkeleton::HandleConfirmUnregisterService_TUInt32},
        {ONDISCONNECT_TUINT32_ID, &IfConnectingRespSkeleton::HandleOnDisconnect_TUInt32},
        {SENDRSAPUBLICKEY_TUINT16_TUINT16_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__ID, &IfConnectingRespSkeleton::HandleSendRSAPublicKey_TUInt16_TUInt16_OctSeq_tpl_begin_TUInt16_tpl_end__OctSeq_tpl_begin_TUInt16_tpl_end_},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 4;
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
TInt32 IfConnectingRespSkeleton::HandleRespRegisterService_CDoid_pt_TUInt32_TInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    CDoid _pDoid;
    nRet = Unmarshall(pBuffer,nLen,_pDoid);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uServiceId;
    nRet = Unmarshall(pBuffer,nLen,_uServiceId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TInt32 _nRslt;
    nRet = Unmarshall(pBuffer,nLen,_nRslt);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespRegisterService(&_pDoid,_uServiceId,_nRslt);
    return SUCCESS;
}
TInt32 IfConnectingRespSkeleton::HandleConfirmUnregisterService_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uServiceId;
    nRet = Unmarshall(pBuffer,nLen,_uServiceId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ConfirmUnregisterService(_uServiceId);
    return SUCCESS;
}
TInt32 IfConnectingRespSkeleton::HandleOnDisconnect_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uReason;
    nRet = Unmarshall(pBuffer,nLen,_uReason);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->OnDisconnect(_uReason);
    return SUCCESS;
}
TInt32 IfConnectingRespSkeleton::HandleSendRSAPublicKey_TUInt16_TUInt16_OctSeq_tpl_begin_TUInt16_tpl_end__OctSeq_tpl_begin_TUInt16_tpl_end_(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt16 _uBits;
    nRet = Unmarshall(pBuffer,nLen,_uBits);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt16 _isFermat4;
    nRet = Unmarshall(pBuffer,nLen,_isFermat4);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    OctSeq<TUInt16> _e;
    nRet = Unmarshall(pBuffer,nLen,_e);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    OctSeq<TUInt16> _n;
    nRet = Unmarshall(pBuffer,nLen,_n);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->SendRSAPublicKey(_uBits,_isFermat4,_e,_n);
    return SUCCESS;
}
}

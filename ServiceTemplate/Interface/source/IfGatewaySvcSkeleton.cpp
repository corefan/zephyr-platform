#include "./IfGatewaySvcSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "IfGatewaySvcMethodId.h"
namespace Zephyr 
{
TInt32 IfGatewaySvcSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfGatewaySvcSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|SYN2MAP_TUINT32_TLV_TPL_BEGIN_TUINT8_AND_TUINT16_TPL_END__REF_ID), &IfGatewaySvcSkeleton::HandleSyn2Map_TUInt32_TLV_tpl_begin_TUInt8_and_TUInt16_tpl_end__ref},
        {(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|REGISTERSERVICE_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID), &IfGatewaySvcSkeleton::HandleRegisterService_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref},
        {(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|UNREGISTERSERVICE_TUINT32_ID), &IfGatewaySvcSkeleton::HandleUnRegisterService_TUInt32},
        {(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|BROADCASTTEAMMSG_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID), &IfGatewaySvcSkeleton::HandleBroadcastTeamMsg_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 4;
    TUInt32 nMethodId = pMsg->GetMethodId();
    _PFMSG pPfMsg = NULL;
    while((nBegin < nEnd)
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
TInt32 IfGatewaySvcSkeleton::Syn2Map_TUInt32_TLV_tpl_begin_TUInt8_and_TUInt16_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 uFrom;
    nRet = Unmarshall(pBuffer,nLen,uFrom);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TLV<TUInt8,TUInt16> tTLV;
    nRet = Unmarshall(pBuffer,nLen,tTLV);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->Syn2Map(uFrom,tTLV);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::RegisterService_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 uServiceId;
    nRet = Unmarshall(pBuffer,nLen,uServiceId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    OctSeq<TUInt16> tServiceName;
    nRet = Unmarshall(pBuffer,nLen,tServiceName);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->RegisterService(uServiceId,tServiceName);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::UnRegisterService_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 uServiceId;
    nRet = Unmarshall(pBuffer,nLen,uServiceId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->UnRegisterService(uServiceId);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::BroadcastTeamMsg_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 uTeam;
    nRet = Unmarshall(pBuffer,nLen,uTeam);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    OctSeq<TUInt16> tServiceName;
    nRet = Unmarshall(pBuffer,nLen,tServiceName);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->BroadcastTeamMsg(uTeam,tServiceName);
    return SUCCESS;
}
}

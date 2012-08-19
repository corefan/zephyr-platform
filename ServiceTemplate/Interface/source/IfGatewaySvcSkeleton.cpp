#include "../include/IfGatewaySvcSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfGatewaySvcMethodId.h"
namespace Zephyr 
{
TInt32 IfGatewaySvcSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfGatewaySvcSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {SYN2MAP_TUINT32_TLV_TPL_BEGIN_TUINT8_AND_TUINT16_TPL_END__REF_ID, &IfGatewaySvcSkeleton::HandleSyn2Map_TUInt32_TLV_tpl_begin_TUInt8_and_TUInt16_tpl_end__ref},
        {REGISTERSERVICE_TUINT32_TUINT32_TUINT32_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID, &IfGatewaySvcSkeleton::HandleRegisterService_TUInt32_TUInt32_TUInt32_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref},
        {UNREGISTERSERVICE_TUINT32_TUINT32_TUINT32_ID, &IfGatewaySvcSkeleton::HandleUnRegisterService_TUInt32_TUInt32_TUInt32},
        {BROADCASTTEAMMSG_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID, &IfGatewaySvcSkeleton::HandleBroadcastTeamMsg_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref},
        {CHANGEPRIORTY_TUINT32_CDOID_PT_TUINT32_ID, &IfGatewaySvcSkeleton::HandleChangePriorty_TUInt32_CDoid_pt_TUInt32},
        {STARTLOGIN_TUINT32_TUINT16_TUINT16_ID, &IfGatewaySvcSkeleton::HandleStartLogin_TUInt32_TUInt16_TUInt16},
        {STOPLOGIN_ID, &IfGatewaySvcSkeleton::HandleStopLogin},
        {DISCONNECTEDALLCLIENT_ID, &IfGatewaySvcSkeleton::HandleDisconnectedAllClient},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 8;
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
TInt32 IfGatewaySvcSkeleton::HandleSyn2Map_TUInt32_TLV_tpl_begin_TUInt8_and_TUInt16_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uFrom;
    nRet = Unmarshall(pBuffer,nLen,_uFrom);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TLV<TUInt8,TUInt16> _tTLV;
    nRet = Unmarshall(pBuffer,nLen,_tTLV);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->Syn2Map(_uFrom,_tTLV);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleRegisterService_TUInt32_TUInt32_TUInt32_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uServiceId;
    nRet = Unmarshall(pBuffer,nLen,_uServiceId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uServicBegin;
    nRet = Unmarshall(pBuffer,nLen,_uServicBegin);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uEnd;
    nRet = Unmarshall(pBuffer,nLen,_uEnd);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uPriority;
    nRet = Unmarshall(pBuffer,nLen,_uPriority);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    OctSeq<TUInt16> _tServiceName;
    nRet = Unmarshall(pBuffer,nLen,_tServiceName);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->RegisterService(_uServiceId,_uServicBegin,_uEnd,_uPriority,_tServiceName);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleUnRegisterService_TUInt32_TUInt32_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uServiceId;
    nRet = Unmarshall(pBuffer,nLen,_uServiceId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uServicBegin;
    nRet = Unmarshall(pBuffer,nLen,_uServicBegin);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uEnd;
    nRet = Unmarshall(pBuffer,nLen,_uEnd);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->UnRegisterService(_uServiceId,_uServicBegin,_uEnd);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleBroadcastTeamMsg_TUInt32_OctSeq_tpl_begin_TUInt16_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uTeam;
    nRet = Unmarshall(pBuffer,nLen,_uTeam);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    OctSeq<TUInt16> _tServiceName;
    nRet = Unmarshall(pBuffer,nLen,_tServiceName);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->BroadcastTeamMsg(_uTeam,_tServiceName);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleChangePriorty_TUInt32_CDoid_pt_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uServiceId;
    nRet = Unmarshall(pBuffer,nLen,_uServiceId);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    CDoid _pMyDoid;
    nRet = Unmarshall(pBuffer,nLen,_pMyDoid);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uPriority;
    nRet = Unmarshall(pBuffer,nLen,_uPriority);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->ChangePriorty(_uServiceId,&_pMyDoid,_uPriority);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleStartLogin_TUInt32_TUInt16_TUInt16(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uIp;
    nRet = Unmarshall(pBuffer,nLen,_uIp);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt16 _nListeningPort;
    nRet = Unmarshall(pBuffer,nLen,_nListeningPort);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt16 _nMaxConnection;
    nRet = Unmarshall(pBuffer,nLen,_nMaxConnection);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->StartLogin(_uIp,_nListeningPort,_nMaxConnection);
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleStopLogin(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    m_pImplementObj->StopLogin();
    return SUCCESS;
}
TInt32 IfGatewaySvcSkeleton::HandleDisconnectedAllClient(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    m_pImplementObj->DisconnectedAllClient();
    return SUCCESS;
}
}

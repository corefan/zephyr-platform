#include "../include/IfConnectingSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfConnectingMethodId.h"
namespace Zephyr 
{
TInt32 IfConnectingSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfConnectingSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {REGISTERSERVICE_CDOID_PT_TUINT32_TUINT32_TUINT32_TUINT32_ID, &IfConnectingSkeleton::HandleRegisterService_CDoid_pt_TUInt32_TUInt32_TUInt32_TUInt32},
        {UNREGISTERSERVICE_TUINT32_TUINT32_TUINT32_ID, &IfConnectingSkeleton::HandleUnregisterService_TUInt32_TUInt32_TUInt32},
        {REGISTERTEAM_TUINT32_ID, &IfConnectingSkeleton::HandleRegisterTeam_TUInt32},
        {DISCONNECT_TUINT32_ID, &IfConnectingSkeleton::HandleDisconnect_TUInt32},
        {SETID_TUINT32_ID, &IfConnectingSkeleton::HandleSetId_TUInt32},
        {CHECKID_TUINT32_ID, &IfConnectingSkeleton::HandleCheckId_TUInt32},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 6;
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
TInt32 IfConnectingSkeleton::HandleRegisterService_CDoid_pt_TUInt32_TUInt32_TUInt32_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    CDoid _pDoid;
    nRet = Unmarshall(pBuffer,nLen,_pDoid);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
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
    TUInt32 _uServiceIdBegin;
    nRet = Unmarshall(pBuffer,nLen,_uServiceIdBegin);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uServcieIdEnd;
    nRet = Unmarshall(pBuffer,nLen,_uServcieIdEnd);
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
    m_pImplementObj->RegisterService(&_pDoid,_uServiceId,_uServiceIdBegin,_uServcieIdEnd,_uPriority);
    return SUCCESS;
}
TInt32 IfConnectingSkeleton::HandleUnregisterService_TUInt32_TUInt32_TUInt32(CMessageHeader *pMsg)
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
    TUInt32 _uServiceIdBegin;
    nRet = Unmarshall(pBuffer,nLen,_uServiceIdBegin);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    TUInt32 _uServcieIdEnd;
    nRet = Unmarshall(pBuffer,nLen,_uServcieIdEnd);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->UnregisterService(_uServiceId,_uServiceIdBegin,_uServcieIdEnd);
    return SUCCESS;
}
TInt32 IfConnectingSkeleton::HandleRegisterTeam_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uTeamID;
    nRet = Unmarshall(pBuffer,nLen,_uTeamID);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->RegisterTeam(_uTeamID);
    return SUCCESS;
}
TInt32 IfConnectingSkeleton::HandleDisconnect_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uReason;
    nRet = Unmarshall(pBuffer,nLen,_uReason);
    if (nRet<SUCCESS)
    {
        pBuffer += nRet;
        nLen -= nRet;
    }
    else
    {
        return nRet;
    }
    m_pImplementObj->Disconnect(_uReason);
    return SUCCESS;
}
TInt32 IfConnectingSkeleton::HandleSetId_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uId;
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
    m_pImplementObj->SetId(_uId);
    return SUCCESS;
}
TInt32 IfConnectingSkeleton::HandleCheckId_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uId;
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
    m_pImplementObj->CheckId(_uId);
    return SUCCESS;
}
}

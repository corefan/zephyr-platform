#include "../include/IfUserSessionSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfUserSessionMethodId.h"
#include "../include/TJobInfosUnmarshaller.h"
namespace erp_platform 
{
TInt32 IfUserSessionSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfUserSessionSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {CHANGEPWD_TCHAR_PT_TCHAR_PT_ID, &IfUserSessionSkeleton::HandleChangePwd_TChar_pt_TChar_pt},
        {REQGETMYINFO_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetMyInfo_TUInt32},
        {REQGETUSERINFO_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetUserInfo_TUInt32},
        {REQGETDEPARTMENTINFO_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetDepartmentInfo_TUInt32},
        {REQGETMENUINFOLIST_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetMenuInfoList_TUInt32},
        {REQUPDATEMENUINFO_TJOBINFOS_REF_ID, &IfUserSessionSkeleton::HandleReqUpdateMenuInfo_TJobInfos_ref},
        {REQGETACTIVEWORKINFO_TINT32_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetActiveWorkInfo_TInt32_TUInt32},
        {REQGETMYWORKLIST_TUINT32_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetMyWorkList_TUInt32_TUInt32},
        {REQGETMENUINFOBYID_TUINT32_ID, &IfUserSessionSkeleton::HandleReqGetMenuInfoById_TUInt32},
    };
    TInt32 nBegin = 0;
    TInt32 nEnd = 9;
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
TInt32 IfUserSessionSkeleton::HandleChangePwd_TChar_pt_TChar_pt(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TChar *_pOldPwd;
    nRet = Unmarshall(pBuffer,nLen,_pOldPwd);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TChar *_pszNewPwd;
    nRet = Unmarshall(pBuffer,nLen,_pszNewPwd);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ChangePwd(_pOldPwd,_pszNewPwd);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetMyInfo_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uLastUpdateTime;
    nRet = Unmarshall(pBuffer,nLen,_uLastUpdateTime);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetMyInfo(_uLastUpdateTime);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetUserInfo_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uUserId;
    nRet = Unmarshall(pBuffer,nLen,_uUserId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetUserInfo(_uUserId);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetDepartmentInfo_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uDepartmentId;
    nRet = Unmarshall(pBuffer,nLen,_uDepartmentId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetDepartmentInfo(_uDepartmentId);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetMenuInfoList_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uFatherId;
    nRet = Unmarshall(pBuffer,nLen,_uFatherId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetMenuInfoList(_uFatherId);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqUpdateMenuInfo_TJobInfos_ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TJobInfos _rJobInfos;
    nRet = Unmarshall(pBuffer,nLen,_rJobInfos);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqUpdateMenuInfo(_rJobInfos);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetActiveWorkInfo_TInt32_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TInt32 _nJobId;
    nRet = Unmarshall(pBuffer,nLen,_nJobId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uRcdId;
    nRet = Unmarshall(pBuffer,nLen,_uRcdId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetActiveWorkInfo(_nJobId,_uRcdId);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetMyWorkList_TUInt32_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uFromTime;
    nRet = Unmarshall(pBuffer,nLen,_uFromTime);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uToTime;
    nRet = Unmarshall(pBuffer,nLen,_uToTime);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetMyWorkList(_uFromTime,_uToTime);
    return SUCCESS;
}
TInt32 IfUserSessionSkeleton::HandleReqGetMenuInfoById_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uMenuId;
    nRet = Unmarshall(pBuffer,nLen,_uMenuId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->ReqGetMenuInfoById(_uMenuId);
    return SUCCESS;
}
}

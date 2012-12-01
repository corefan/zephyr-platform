#include "../include/IfUsersessionRespSkeleton.h"
#include "Public/include/TypeUnmarshaller.h"
#include "../include/IfUsersessionRespMethodId.h"
#include "../include/TJobInfosUnmarshaller.h"
namespace erp_platform 
{
TInt32 IfUsersessionRespSkeleton::HandleMsg(CMessageHeader *pMsg)
{
    typedef TInt32 (IfUsersessionRespSkeleton::*_PFMSG)(CMessageHeader *); 
    struct _MSGMAP_ENTRY { TUInt32 m_uMsgID; _PFMSG m_pHandlerFunc; };
    static _MSGMAP_ENTRY sMsgMapEntries[] = 
    {
        {ONPWDCHANGED_TINT32_ID, &IfUsersessionRespSkeleton::HandleOnPwdChanged_TInt32},
        {RESPGETMYINFO_TUINT32_TCHAR_PT_TCHAR_PT_TUINT32_TUINT32_ID, &IfUsersessionRespSkeleton::HandleRespGetMyInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32},
        {RESPGETUSERINFO_TUINT32_TCHAR_PT_TCHAR_PT_TUINT32_TUINT32_ID, &IfUsersessionRespSkeleton::HandleRespGetUserInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32},
        {RESPGETDEPARTINFO_TUINT32_TCHAR_PT_TUINT32_ID, &IfUsersessionRespSkeleton::HandleRespGetDepartInfo_TUInt32_TChar_pt_TUInt32},
        {SENDJOBLIST_LIST_TPL_BEGIN_TJOBINFOS_TPL_END__REF_LIST_TPL_BEGIN_TJOBINFOS_TPL_END__REF_ID, &IfUsersessionRespSkeleton::HandleSendJobList_list_tpl_begin_TJobInfos_tpl_end__ref_list_tpl_begin_TJobInfos_tpl_end__ref},
        {SENDACTIVEWORK_TJOBINFOS_TUINT32_CONST_TCHAR_PT_ID, &IfUsersessionRespSkeleton::HandleSendActiveWork_TJobInfos_TUInt32_const_TChar_pt},
        {RESPMENULIST_LIST_TPL_BEGIN_TJOBINFOS_TPL_END__ID, &IfUsersessionRespSkeleton::HandleRespMenuList_list_tpl_begin_TJobInfos_tpl_end_},
        {RESPGETMENUINFO_TJOBINFOS_REF_ID, &IfUsersessionRespSkeleton::HandleRespGetMenuInfo_TJobInfos_ref},
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
TInt32 IfUsersessionRespSkeleton::HandleOnPwdChanged_TInt32(CMessageHeader *pMsg)
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
    m_pImplementObj->OnPwdChanged(_nResult);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleRespGetMyInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uUpdateTime;
    nRet = Unmarshall(pBuffer,nLen,_uUpdateTime);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TChar *_pszName;
    nRet = Unmarshall(pBuffer,nLen,_pszName);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TChar *_pszApartName;
    nRet = Unmarshall(pBuffer,nLen,_pszApartName);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uMyId;
    nRet = Unmarshall(pBuffer,nLen,_uMyId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uApartId;
    nRet = Unmarshall(pBuffer,nLen,_uApartId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespGetMyInfo(_uUpdateTime,_pszName,_pszApartName,_uMyId,_uApartId);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleRespGetUserInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uUpdateTime;
    nRet = Unmarshall(pBuffer,nLen,_uUpdateTime);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TChar *_pszName;
    nRet = Unmarshall(pBuffer,nLen,_pszName);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TChar *_pszApartName;
    nRet = Unmarshall(pBuffer,nLen,_pszApartName);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uMyId;
    nRet = Unmarshall(pBuffer,nLen,_uMyId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uApartId;
    nRet = Unmarshall(pBuffer,nLen,_uApartId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespGetUserInfo(_uUpdateTime,_pszName,_pszApartName,_uMyId,_uApartId);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleRespGetDepartInfo_TUInt32_TChar_pt_TUInt32(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TUInt32 _uUpdateTime;
    nRet = Unmarshall(pBuffer,nLen,_uUpdateTime);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TChar *_pszApartName;
    nRet = Unmarshall(pBuffer,nLen,_pszApartName);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    TUInt32 _uApartId;
    nRet = Unmarshall(pBuffer,nLen,_uApartId);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespGetDepartInfo(_uUpdateTime,_pszApartName,_uApartId);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleSendJobList_list_tpl_begin_TJobInfos_tpl_end__ref_list_tpl_begin_TJobInfos_tpl_end__ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    list<TJobInfos> _tMenu2;
    nRet = Unmarshall(pBuffer,nLen,_tMenu2);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    list<TJobInfos> _tMeun3;
    nRet = Unmarshall(pBuffer,nLen,_tMeun3);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->SendJobList(_tMenu2,_tMeun3);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleSendActiveWork_TJobInfos_TUInt32_const_TChar_pt(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TJobInfos _tJobs;
    nRet = Unmarshall(pBuffer,nLen,_tJobs);
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
    TChar *_pszTitle;
    nRet = Unmarshall(pBuffer,nLen,_pszTitle);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->SendActiveWork(_tJobs,_uRcdId,_pszTitle);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleRespMenuList_list_tpl_begin_TJobInfos_tpl_end_(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    list<TJobInfos> _tList;
    nRet = Unmarshall(pBuffer,nLen,_tList);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespMenuList(_tList);
    return SUCCESS;
}
TInt32 IfUsersessionRespSkeleton::HandleRespGetMenuInfo_TJobInfos_ref(CMessageHeader *pMsg)
{
    TInt32 nLen = pMsg->GetBodyLength();
    TUChar *pBuffer =pMsg->GetBody();
    TInt32 nRet;
    TJobInfos _tJobInfo;
    nRet = Unmarshall(pBuffer,nLen,_tJobInfo);
    if (nRet<SUCCESS)
    {
        return nRet;
    }
    pBuffer += nRet;
    nLen -= nRet;
    m_pImplementObj->RespGetMenuInfo(_tJobInfo);
    return SUCCESS;
}
}

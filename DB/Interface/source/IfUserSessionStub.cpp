#include "../include/IfUserSessionStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfUserSessionMethodId.h"
#include "../include/TJobInfosMarshaller.h"
namespace erp_platform 
{
TInt32 IfUserSessionStub::ChangePwd(TChar* _pOldPwd,TChar* _pszNewPwd)
{
    TInt32 nLen = GetLength(_pOldPwd)+GetLength(_pszNewPwd);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CHANGEPWD_TCHAR_PT_TCHAR_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_pOldPwd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszNewPwd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetMyInfo(TUInt32 _uLastUpdateTime)
{
    TInt32 nLen = GetLength(_uLastUpdateTime);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETMYINFO_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uLastUpdateTime);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetUserInfo(TUInt32 _uUserId)
{
    TInt32 nLen = GetLength(_uUserId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETUSERINFO_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uUserId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetDepartmentInfo(TUInt32 _uDepartmentId)
{
    TInt32 nLen = GetLength(_uDepartmentId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETDEPARTMENTINFO_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uDepartmentId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetMenuInfoList(TUInt32 _uFatherId)
{
    TInt32 nLen = GetLength(_uFatherId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETMENUINFOLIST_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uFatherId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqUpdateMenuInfo(TJobInfos& _rJobInfos)
{
    TInt32 nLen = GetLength(_rJobInfos);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQUPDATEMENUINFO_TJOBINFOS_REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_rJobInfos);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetActiveWorkInfo(TInt32 _nJobId,TUInt32 _uRcdId)
{
    TInt32 nLen = GetLength(_nJobId)+GetLength(_uRcdId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETACTIVEWORKINFO_TINT32_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_nJobId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uRcdId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetMyWorkList(TUInt32 _uFromTime,TUInt32 _uToTime)
{
    TInt32 nLen = GetLength(_uFromTime)+GetLength(_uToTime);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETMYWORKLIST_TUINT32_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uFromTime);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uToTime);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUserSessionStub::ReqGetMenuInfoById(TUInt32 _uMenuId)
{
    TInt32 nLen = GetLength(_uMenuId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETMENUINFOBYID_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uMenuId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

}

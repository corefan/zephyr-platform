#include "../include/IfUsersessionRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfUsersessionRespMethodId.h"
#include "../include/TJobInfosMarshaller.h"
namespace erp_platform 
{
TInt32 IfUsersessionRespStub::OnPwdChanged(TInt32 _nResult)
{
    TInt32 nLen = GetLength(_nResult);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(ONPWDCHANGED_TINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_nResult);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::RespGetMyInfo(TUInt32 _uUpdateTime,TChar* _pszName,TChar* _pszApartName,TUInt32 _uMyId,TUInt32 _uApartId)
{
    TInt32 nLen = GetLength(_uUpdateTime)+GetLength(_pszName)+GetLength(_pszApartName)+GetLength(_uMyId)+GetLength(_uApartId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPGETMYINFO_TUINT32_TCHAR_PT_TCHAR_PT_TUINT32_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uUpdateTime);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszApartName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uMyId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uApartId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::RespGetUserInfo(TUInt32 _uUpdateTime,TChar* _pszName,TChar* _pszApartName,TUInt32 _uMyId,TUInt32 _uApartId)
{
    TInt32 nLen = GetLength(_uUpdateTime)+GetLength(_pszName)+GetLength(_pszApartName)+GetLength(_uMyId)+GetLength(_uApartId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPGETUSERINFO_TUINT32_TCHAR_PT_TCHAR_PT_TUINT32_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uUpdateTime);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszApartName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uMyId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uApartId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::RespGetDepartInfo(TUInt32 _uUpdateTime,TChar* _pszApartName,TUInt32 _uApartId)
{
    TInt32 nLen = GetLength(_uUpdateTime)+GetLength(_pszApartName)+GetLength(_uApartId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPGETDEPARTINFO_TUINT32_TCHAR_PT_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uUpdateTime);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszApartName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uApartId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::SendJobList(list<TJobInfos>& _tMenu2,list<TJobInfos>& _tMeun3)
{
    TInt32 nLen = GetLength(_tMenu2)+GetLength(_tMeun3);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(SENDJOBLIST_LIST_TPL_BEGIN_TJOBINFOS_TPL_END__REF_LIST_TPL_BEGIN_TJOBINFOS_TPL_END__REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_tMenu2);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_tMeun3);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::SendActiveWork(TJobInfos _tJobs,TUInt32 _uRcdId,const TChar* _pszTitle)
{
    TInt32 nLen = GetLength(_tJobs)+GetLength(_uRcdId)+GetLength(_pszTitle);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(SENDACTIVEWORK_TJOBINFOS_TUINT32_CONST_TCHAR_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_tJobs);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_pszTitle);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::RespMenuList(list<TJobInfos> _tList)
{
    TInt32 nLen = GetLength(_tList);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPMENULIST_LIST_TPL_BEGIN_TJOBINFOS_TPL_END__ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_tList);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfUsersessionRespStub::RespGetMenuInfo(TJobInfos& _tJobInfo)
{
    TInt32 nLen = GetLength(_tJobInfo);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPGETMENUINFO_TJOBINFOS_REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_tJobInfo);
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

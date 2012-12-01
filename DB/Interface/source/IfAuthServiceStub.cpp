#include "../include/IfAuthServiceStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfAuthServiceMethodId.h"
namespace erp_platform 
{
TInt32 IfAuthServiceStub::Authenticate(TUInt32 _uDyncNr,TChar* _pszName,TChar* _pszPwd)
{
    TInt32 nLen = GetLength(_uDyncNr)+GetLength(_pszName)+GetLength(_pszPwd);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(AUTHENTICATE_TUINT32_TCHAR_PT_TCHAR_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uDyncNr);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_pszPwd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfAuthServiceStub::ChangePwd(TChar* _pszName,TChar* _pszOldPwd,TChar* _pNewPwd)
{
    TInt32 nLen = GetLength(_pszName)+GetLength(_pszOldPwd)+GetLength(_pNewPwd);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CHANGEPWD_TCHAR_PT_TCHAR_PT_TCHAR_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_pszName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pszOldPwd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_pNewPwd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfAuthServiceStub::OnDisconneted(CDoid _tMyDoid)
{
    TInt32 nLen = GetLength(_tMyDoid);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(ONDISCONNETED_CDOID_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_tMyDoid);
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

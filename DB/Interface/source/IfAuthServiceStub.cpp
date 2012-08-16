#include "../include/IfAuthServiceStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfAuthServiceMethodId.h"
namespace Zephyr 
{
TInt32 IfAuthServiceStub::Authenticate(TUInt32 _uIp,TChar* _pszName,TChar* _pszPwd)
{
    TInt32 nLen = GetLength(_uIp)+GetLength(_pszName)+GetLength(_pszPwd);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(AUTHENTICATE_TUINT32_TCHAR_PT_TCHAR_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uIp);
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

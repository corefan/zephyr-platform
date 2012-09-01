#include "../include/IfConnectingRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfConnectingRespMethodId.h"
namespace Zephyr 
{
TInt32 IfConnectingRespStub::RespRegisterService(CDoid* _pDoid,TUInt32 _uServiceId,TInt32 _nRslt)
{
    TInt32 nLen = GetLength(_pDoid)+GetLength(_uServiceId)+GetLength(_nRslt);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPREGISTERSERVICE_CDOID_PT_TUINT32_TINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_pDoid);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uServiceId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_nRslt);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingRespStub::ConfirmUnregisterService(TUInt32 _uServiceId)
{
    TInt32 nLen = GetLength(_uServiceId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CONFIRMUNREGISTERSERVICE_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uServiceId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingRespStub::OnDisconnect(TUInt32 _uReason)
{
    TInt32 nLen = GetLength(_uReason);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(ONDISCONNECT_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uReason);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingRespStub::SendRSAPublicKey(TUInt16 _uBits,TUInt16 _isFermat4,OctSeq<TUInt16> _e,OctSeq<TUInt16> _n)
{
    TInt32 nLen = GetLength(_uBits)+GetLength(_isFermat4)+GetLength(_e)+GetLength(_n);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(SENDRSAPUBLICKEY_TUINT16_TUINT16_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uBits);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_isFermat4);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_e);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_n);
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

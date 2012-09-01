#include "../include/IfConnectingStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfConnectingMethodId.h"
namespace Zephyr 
{
TInt32 IfConnectingStub::RegisterService(CDoid* _pDoid,TUInt32 _uServiceId,TUInt32 _uServiceIdBegin,TUInt32 _uServcieIdEnd,TUInt32 _uPriority)
{
    TInt32 nLen = GetLength(_pDoid)+GetLength(_uServiceId)+GetLength(_uServiceIdBegin)+GetLength(_uServcieIdEnd)+GetLength(_uPriority);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REGISTERSERVICE_CDOID_PT_TUINT32_TUINT32_TUINT32_TUINT32_ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_uServiceIdBegin);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uServcieIdEnd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uPriority);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingStub::UnregisterService(TUInt32 _uServiceId,TUInt32 _uServiceIdBegin,TUInt32 _uServcieIdEnd)
{
    TInt32 nLen = GetLength(_uServiceId)+GetLength(_uServiceIdBegin)+GetLength(_uServcieIdEnd);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(UNREGISTERSERVICE_TUINT32_TUINT32_TUINT32_ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_uServiceIdBegin);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uServcieIdEnd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingStub::RegisterTeam(TUInt32 _uTeamID)
{
    TInt32 nLen = GetLength(_uTeamID);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REGISTERTEAM_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uTeamID);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingStub::Disconnect(TUInt32 _uReason)
{
    TInt32 nLen = GetLength(_uReason);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(DISCONNECT_TUINT32_ID),&m_tTarget,1,false);
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

TInt32 IfConnectingStub::SetId(TUInt32 _uId)
{
    TInt32 nLen = GetLength(_uId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(SETID_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingStub::CheckId(TUInt32 _uId)
{
    TInt32 nLen = GetLength(_uId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CHECKID_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfConnectingStub::SendCryptedKey(OctSeq<TUInt16> _tKey)
{
    TInt32 nLen = GetLength(_tKey);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(SENDCRYPTEDKEY_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_tKey);
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

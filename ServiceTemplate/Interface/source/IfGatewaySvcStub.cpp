#include "../include/IfGatewaySvcStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfGatewaySvcMethodId.h"
namespace Zephyr 
{
TInt32 IfGatewaySvcStub::Syn2Map(TUInt32 _uFrom,TLV<TUInt8,TUInt16>& _tTLV)
{
    TInt32 nLen = GetLength(_uFrom)+GetLength(_tTLV);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(SYN2MAP_TUINT32_TLV_TPL_BEGIN_TUINT8_AND_TUINT16_TPL_END__REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uFrom);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_tTLV);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::RegisterService(TUInt32 _uServiceId,TUInt32 _uServicBegin,TUInt32 _uEnd,TUInt32 _uPriority,OctSeq<TUInt16>& _tServiceName)
{
    TInt32 nLen = GetLength(_uServiceId)+GetLength(_uServicBegin)+GetLength(_uEnd)+GetLength(_uPriority)+GetLength(_tServiceName);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REGISTERSERVICE_TUINT32_TUINT32_TUINT32_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_uServicBegin);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uEnd);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_tServiceName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::UnRegisterService(TUInt32 _uServiceId,TUInt32 _uServicBegin,TUInt32 _uEnd)
{
    TInt32 nLen = GetLength(_uServiceId)+GetLength(_uServicBegin)+GetLength(_uEnd);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_uServicBegin);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_uEnd);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::BroadcastTeamMsg(TUInt32 _uTeam,OctSeq<TUInt16>& _tServiceName)
{
    TInt32 nLen = GetLength(_uTeam)+GetLength(_tServiceName);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(BROADCASTTEAMMSG_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uTeam);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_tServiceName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::ChangePriorty(TUInt32 _uServiceId,CDoid* _pMyDoid,TUInt32 _uPriority)
{
    TInt32 nLen = GetLength(_uServiceId)+GetLength(_pMyDoid)+GetLength(_uPriority);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CHANGEPRIORTY_TUINT32_CDOID_PT_TUINT32_ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_pMyDoid);
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

TInt32 IfGatewaySvcStub::StartLogin(TUInt32 _uIp,TUInt16 _nListeningPort,TUInt16 _nMaxConnection)
{
    TInt32 nLen = GetLength(_uIp)+GetLength(_nListeningPort)+GetLength(_nMaxConnection);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(STARTLOGIN_TUINT32_TUINT16_TUINT16_ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_nListeningPort);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_nMaxConnection);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::StopLogin()
{
    TInt32 nLen = 0;
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(STOPLOGIN_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::DisconnectedAllClient()
{
    TInt32 nLen = 0;
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(DISCONNECTEDALLCLIENT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    return m_pOnwerObj->SendMsg(pMsg);
}

}

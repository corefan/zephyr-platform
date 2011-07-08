#include "./IfGatewaySvcStub.h"
#include "Public/include/TypeMarshaller.h"
#include "IfGatewaySvcMethodId.h"
namespace Zephyr 
{
TInt32 IfGatewaySvcStub::Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV)
{
    TInt32 nLen = sizeof(TUInt32)+sizeof(TLV<TUInt8,TUInt16>);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|SYN2MAP_TUINT32_TLV_TPL_BEGIN_TUINT8_AND_TUINT16_TPL_END__REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uFrom);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nRet = Marshall(pBuffer+nUsed,nLen,tTLV);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName)
{
    TInt32 nLen = sizeof(TUInt32)+sizeof(OctSeq<TUInt16>);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|REGISTERSERVICE_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uServiceId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nRet = Marshall(pBuffer+nUsed,nLen,tServiceName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::UnRegisterService(TUInt32 uServiceId)
{
    TInt32 nLen = sizeof(TUInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|UNREGISTERSERVICE_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uServiceId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfGatewaySvcStub::BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName)
{
    TInt32 nLen = sizeof(TUInt32)+sizeof(OctSeq<TUInt16>);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFGATEWAYSVC_INTERFACE_ID|BROADCASTTEAMMSG_TUINT32_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__REF_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uTeam);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nRet = Marshall(pBuffer+nUsed,nLen,tServiceName);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

}

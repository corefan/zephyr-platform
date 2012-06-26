#include "../include/IfConnectingRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfConnectingRespMethodId.h"
namespace Zephyr 
{
TInt32 IfConnectingRespStub::RespRegisterService(CDoid* pDoid,TUInt32 uServiceId,TInt32 nRslt)
{
    TInt32 nLen = sizeof(CDoid)+sizeof(TUInt32)+sizeof(TInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPREGISTERSERVICE_CDOID_PT_TUINT32_TINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,pDoid);
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
    nRet = Marshall(pBuffer+nUsed,nLen,nRslt);
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

TInt32 IfConnectingRespStub::ConfirmUnregisterService(TUInt32 uServiceId)
{
    TInt32 nLen = sizeof(TUInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CONFIRMUNREGISTERSERVICE_TUINT32_ID),&m_tTarget,1,false);
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

TInt32 IfConnectingRespStub::OnDisconnect(TUInt32 uReason)
{
    TInt32 nLen = sizeof(TUInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(ONDISCONNECT_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uReason);
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

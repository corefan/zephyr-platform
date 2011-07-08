#include "../include/IfConnectingStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfConnectingMethodId.h"
#include "Public/include/Doid.h"
namespace Zephyr 
{
TInt32 IfConnectingStub::RegisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid* pDoid)
{
    TInt32 nLen = sizeof(TUInt32)+sizeof(TUInt32)+sizeof(CDoid);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFCONNECTING_INTERFACE_ID|REGISTERSERVICE_TUINT32_TUINT32_CDOID_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uServiceIdBegin);
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
    nRet = Marshall(pBuffer+nUsed,nLen,uServcieIdEnd);
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
    nRet = Marshall(pBuffer+nUsed,nLen,(CDoid*)pDoid);
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

TInt32 IfConnectingStub::UnregisterService(TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,CDoid* pDoid)
{
    TInt32 nLen = sizeof(TUInt32)+sizeof(TUInt32)+sizeof(CDoid);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFCONNECTING_INTERFACE_ID|UNREGISTERSERVICE_TUINT32_TUINT32_CDOID_PT_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uServiceIdBegin);
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
    nRet = Marshall(pBuffer+nUsed,nLen,uServcieIdEnd);
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
    nRet = Marshall(pBuffer+nUsed,nLen,(CDoid*)pDoid);
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

TInt32 IfConnectingStub::Disconnect(TUInt32 uReason)
{
    TInt32 nLen = sizeof(TUInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFCONNECTING_INTERFACE_ID|DISCONNECT_TUINT32_ID),&m_tTarget,1,false);
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

TInt32 IfConnectingStub::SetId(TUInt32 uId)
{
    TInt32 nLen = sizeof(TUInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFCONNECTING_INTERFACE_ID|SETID_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uId);
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

TInt32 IfConnectingStub::CheckId(TUInt32 uId)
{
    TInt32 nLen = sizeof(TUInt32);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(GATEWAY_SERVICE_ID|IFCONNECTING_INTERFACE_ID|CHECKID_TUINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,uId);
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

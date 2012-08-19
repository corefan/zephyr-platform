#include "../include/IfGatewayRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfGatewayRespMethodId.h"
namespace Zephyr 
{
TInt32 IfGatewayRespStub::ConfirmRegisterService(TUInt32 _uServiceId)
{
    TInt32 nLen = GetLength(_uServiceId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CONFIRMREGISTERSERVICE_TUINT32_ID),&m_tTarget,1,false);
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

TInt32 IfGatewayRespStub::ConfirmUnRegisterService(TUInt32 _uServiceId)
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

}

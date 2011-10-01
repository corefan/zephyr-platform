#include "../include/IfAuthServiceStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfAuthServiceMethodId.h"
namespace Zephyr 
{
TInt32 IfAuthServiceStub::Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData)
{
    TInt32 nLen = sizeof(TLV<TUInt16,TUInt16>);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHSERVICE_INTERFACE_ID|AUTHENTICATE_TLV_TPL_BEGIN_TUINT16_AND_TUINT16_TPL_END__ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,tAuthenticateData);
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

TInt32 IfAuthServiceStub::OnDisconneted(CDoid tMyDoid)
{
    TInt32 nLen = sizeof(CDoid);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHSERVICE_INTERFACE_ID|ONDISCONNETED_CDOID_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,tMyDoid);
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

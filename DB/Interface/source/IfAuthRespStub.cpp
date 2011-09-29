#include "../include/IfAuthRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfAuthRespMethodId.h"
namespace Zephyr 
{
TInt32 IfAuthRespStub::RespAuthenticate(TInt32 nResult,TLV<TUInt16,TUInt16> tAuthorityData)
{
    TInt32 nLen = sizeof(TInt32)+sizeof(TLV<TUInt16,TUInt16>);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(AUTHENTICATE_SERVICE_SERVICE_ID|IFAUTHRESP_INTERFACE_ID|RESPAUTHENTICATE_TINT32_TLV_TPL_BEGIN_TUINT16_AND_TUINT16_TPL_END__ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,nResult);
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
    nRet = Marshall(pBuffer+nUsed,nLen,tAuthorityData);
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

#include "../include/IfAuthRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfAuthRespMethodId.h"
namespace Zephyr 
{
TInt32 IfAuthRespStub::RespAuthenticate(TInt32 _nResult)
{
    TInt32 nLen = GetLength(_nResult);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPAUTHENTICATE_TINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_nResult);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfAuthRespStub::ConfirmDisconneted(CDoid _tMyDoid)
{
    TInt32 nLen = GetLength(_tMyDoid);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(CONFIRMDISCONNETED_CDOID_ID),&m_tTarget,1,false);
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

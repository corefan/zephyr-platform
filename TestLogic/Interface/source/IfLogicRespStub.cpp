#include "../include/IfLogicRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfLogicRespMethodId.h"
namespace test_logic 
{
TInt32 IfLogicRespStub::RespTest(TInt32 _nRslt)
{
    TInt32 nLen = GetLength(_nRslt);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPTEST_TINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
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

}

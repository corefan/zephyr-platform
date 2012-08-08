#include "../include/IfLogicStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfLogicMethodId.h"
namespace test_logic 
{
TInt32 IfLogicStub::Test(TUInt64 _uParm1,TInt32 _n2)
{
    TInt32 nLen = GetLength(_uParm1)+GetLength(_n2);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(TEST_TUINT64_TINT32_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uParm1);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_n2);
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

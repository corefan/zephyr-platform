#include "../include/IfLogicServiceRespStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfLogicServiceRespMethodId.h"
namespace test_logic 
{
TInt32 IfLogicServiceRespStub::RespGetSession(TUInt64 _uId,CDoid _tGwDoid)
{
    TInt32 nLen = GetLength(_uId)+GetLength(_tGwDoid);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPGETSESSION_TUINT64_CDOID_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uId);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_tGwDoid);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfLogicServiceRespStub::RespReleaseSession(TUInt64 _uId)
{
    TInt32 nLen = GetLength(_uId);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(RESPRELEASESESSION_TUINT64_ID),&m_tTarget,1,false);
    if (NULL == pMsg)
    {
        return OUT_OF_MEM;
    }
    TUInt32 nUsed=0;
    TInt32 nRet=0;
    TUChar *pBuffer = pMsg->GetBody();
    nRet = Marshall(pBuffer+nUsed,nLen,_uId);
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

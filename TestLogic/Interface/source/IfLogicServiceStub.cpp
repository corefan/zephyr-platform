#include "../include/IfLogicServiceStub.h"
#include "Public/include/TypeMarshaller.h"
#include "../include/IfLogicServiceMethodId.h"
namespace test_logic 
{
TInt32 IfLogicServiceStub::ReqGetSession(TUInt64 _uId,CDoid _tGwDoid,OctSeq<TUInt16> _tData)
{
    TInt32 nLen = GetLength(_uId)+GetLength(_tGwDoid)+GetLength(_tData);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQGETSESSION_TUINT64_CDOID_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_tData);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    pMsg->ResetBodyLength(nUsed);
    return m_pOnwerObj->SendMsg(pMsg);
}

TInt32 IfLogicServiceStub::ReqReleaseSession(TUInt64 _uId,CDoid _tSess,OctSeq<TUInt16> _tData)
{
    TInt32 nLen = GetLength(_uId)+GetLength(_tSess)+GetLength(_tData);
    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(REQRELEASESESSION_TUINT64_CDOID_OCTSEQ_TPL_BEGIN_TUINT16_TPL_END__ID),&m_tTarget,1,false);
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
    nRet = Marshall(pBuffer+nUsed,nLen,_tSess);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    nUsed += nRet;
    nLen-=nRet;
    nRet = Marshall(pBuffer+nUsed,nLen,_tData);
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

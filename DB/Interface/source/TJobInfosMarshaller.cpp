#include "Public/include/TypeMarshaller.h"
#include "../include/TJobInfosMarshaller.h"
#include "../include/TJobInfosGetLength.h"
TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const TJobInfos *_pT)
{
    return Marshall(_pBuff,_nLength,*_pT);
}
TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const TJobInfos &_rValue)
{
    TInt32 nUsed=0;
    TInt32 n = 0;
    n = Marshall(_pBuff+nUsed,_nLength,_rValue.m_uMenuId);

    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    n = Marshall(_pBuff+nUsed,_nLength,_rValue.m_uLvl);

    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    n = Marshall(_pBuff+nUsed,_nLength,_rValue.m_uFatherMenuId);

    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    n = Marshall(_pBuff+nUsed,_nLength,_rValue.m_pszNames);

    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    return nUsed;
}

#include "Public/include/TypeUnmarshaller.h"
#include "../include/TJobInfosUnmarshaller.h"
#include "../include/TJobInfosGetLength.h"
TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,TJobInfos *&_pT)
{
    return Unmarshall(_pBuff,_nLength,*_pT);
}
TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,TJobInfos &_rValue)
{
    TInt32 nUsed=0;
    TInt32 n = 0;
    n = Unmarshall(_pBuff+nUsed,_nLength,_rValue.m_uMenuId);
    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    n = Unmarshall(_pBuff+nUsed,_nLength,_rValue.m_uLvl);
    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    n = Unmarshall(_pBuff+nUsed,_nLength,_rValue.m_uFatherMenuId);
    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    n = Unmarshall(_pBuff+nUsed,_nLength,_rValue.m_pszNames);
    if (n < SUCCESS)
    {
      return n;
    }
    nUsed += n;
    _nLength-=n;
    return nUsed;
}

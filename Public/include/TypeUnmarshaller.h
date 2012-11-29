#ifndef __ZEPHYR_PUBLIC_TYPE_UNMARSHALLER_H__
#define __ZEPHYR_PUBLIC_TYPE_UNMARSHALLER_H__

#include "TypeDef.h"
#include "SysMacros.h"
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;


namespace Zephyr
{
#ifndef _USE_LINK_2_MARSHALL
#define DECLARE_UNMARSHALLERS(TYPE) \
    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBufferLen,TYPE &tType) \
    { \
    if (uBufferLen >= sizeof(TYPE)) \
    { \
    tType = *((TYPE*)pBuffer); \
    return sizeof(TYPE); \
} \
    return OUT_OF_MEM; \
}



    DECLARE_UNMARSHALLERS(TChar) 

        //IMPL_MARSHALLERS(TInt8) 

    DECLARE_UNMARSHALLERS(TUChar) 

        //IMPL_MARSHALLERS(TUInt8) 

    DECLARE_UNMARSHALLERS(TInt16) 

    DECLARE_UNMARSHALLERS(TUInt16) 

    DECLARE_UNMARSHALLERS(TInt32) 

    DECLARE_UNMARSHALLERS(TUInt32) 

    DECLARE_UNMARSHALLERS(TFloat) 

    DECLARE_UNMARSHALLERS(TDouble) 

    DECLARE_UNMARSHALLERS(TInt64) 

    DECLARE_UNMARSHALLERS(TUInt64) 

    DECLARE_UNMARSHALLERS(TBOOL) 
#else

     TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUChar& ucChar);
 
     TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUChar& ucChar);
 
 
     // inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt8 nInt8);
     // inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt8& nInt8);
     // 
     // inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt8 uInt8);
     // inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt8& uInt8);
 
 
     TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt16& nInt16);
 
     TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt16& uInt16);
 
     TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt32& nInt32);
 
      TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt32& uInt32);
 
      TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TFloat& fFloat);
 
      TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TDouble& dDouble);
 
      TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt64& llInt64);
 
      TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt64& ullInt64);
 
      TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TBOOL& bBool);
#endif

TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,string &psz);
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TChar *&psz);
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,const TChar *&psz);
    template<class TYPE>
    TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,OctSeq<TYPE> &tOctSeq)
    {
        tOctSeq.m_nBodyLength = *((TYPE*)pBuffer);
        TInt32 nLength = tOctSeq.GetFullLength();
        if (nLength > uBuffLen)
        {
            return OUT_OF_MEM;
        }
        tOctSeq.m_pBuffer = (pBuffer+sizeof(TYPE));
        return nLength;
    }

    template<class LENGTH_TYPE,class TAG_TYPE>
    TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TLV<LENGTH_TYPE,TAG_TYPE> &tTLV)
    {
        *((LENGTH_TYPE*)pBuffer) = tTLV.m_nBodyLength;
        TInt32 nLength = tTLV.GetFullLength();
        if (nLength > uBuffLen)
        {
            return OUT_OF_MEM;
        }
        pBuffer += sizeof(LENGTH_TYPE);
        tTLV.m_nTag = *((TAG_TYPE*)pBuffer);
        tTLV.m_pBuffer = pBuffer + sizeof(TAG_TYPE);
        return nLength;
    }
}
 
template<class TYPE>
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,vector<TYPE> &tVector)
{
    if (sizeof(TInt32) > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    TInt32 nLen = *((TInt32*)pBuffer);
    tVector.resize(nLen);
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);
        for(TInt32 i=0;i<nLen;++i)
        {
            TInt32 nRet = Unmarshall(pBuffer+nUsed,uBuffLen-nUsed,tVector[i]);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class TYPE>
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,list<TYPE> &tList)
{
    if (sizeof(TInt32) > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    TInt32 nLen = *((TInt32*)pBuffer);
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);

        for(TInt32 i=0;i<nLen;++i)
        {
            TYPE t;
            TInt32 nRet = Unmarshall(pBuffer+nUsed,uBuffLen-nUsed,t);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            tList.push_back(t);
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}


template<class TYPE>
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,set<TYPE> &tSet)
{
    if (sizeof(TInt32) > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    TInt32 nLen = *((TInt32*)pBuffer);
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);

        for(TInt32 i=0;i<nLen;++i)
        {
            TYPE t;
            TInt32 nRet = Unmarshall(pBuffer+nUsed,uBuffLen-nUsed,t);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            tSet.insert(t);
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class CKEY,class TYPE>
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,map<CKEY,TYPE> &tMap)
{
    if (sizeof(TInt32) > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    TInt32 nLen = *((TInt32*)pBuffer);
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);

        for(TInt32 i=0;i<nLen;++i)
        {
            CKEY tKey;
            TInt32 nRet = Unmarshall(pBuffer+nUsed,uBuffLen-nUsed,tKey);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;

            TYPE t;
            nRet = Unmarshall(pBuffer+nUsed,uBuffLen-nUsed,t);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            tMap[tKey] = t;
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}



#endif

#ifndef __ZEPHYR_PUBLIC_TYPE_MARSHALLER_H__
#define __ZEPHYR_PUBLIC_TYPE_MARSHALLER_H__

#include "TypeDef.h"
#include "SysMacros.h"
#include "string.h"
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

namespace Zephyr
{
#ifndef _USE_LINK_2_MARSHALL

#define DECLARE_MARSHALLERS(TYPE) \
    inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBufferLen,const TYPE tType) \
    { \
        if (uBufferLen >= sizeof( TYPE )) \
        { \
            *(( TYPE *)pBuffer) = tType; \
            return sizeof( TYPE ); \
        } \
        return OUT_OF_MEM; \
    } \
    inline TInt32 GetLength(const TYPE c) \
    { \
    return sizeof(TYPE); \
    } \
    inline TInt32 GetLength(const TYPE *pC)\
    {\
        return sizeof(TYPE);\
    }



    inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBufferLen,const TChar tType) 
    { 
        if (uBufferLen >= sizeof( TChar )) 
        { 
            *(( TChar *)pBuffer) = tType; 
            return sizeof( TChar ); 
        } 
        return OUT_OF_MEM; 
    } 
    inline TInt32 GetLength(const TChar c) 
    { 
        return sizeof(TChar); 
    }
    //inline TInt32 GetLength(TChar *pC);
    inline TInt32 GetLength(TChar *psz)
    {
        TInt32 n = strlen(psz);
        return (n+(sizeof(TInt32)+sizeof(TChar)));
    }

    inline TInt32 GetLength(const TChar *psz)
    {
        TInt32 n = strlen(psz);
        return (n+(sizeof(TInt32)+sizeof(TChar)));
    }

    inline TInt32 GetLength(const string &psz)
    {
        return psz.length()+sizeof(TInt32)+sizeof(TChar);
    }

    inline TInt32 GetLength(string &psz)
    {
        return psz.length()+sizeof(TInt32)+sizeof(TChar);
    }


        //IMPL_MARSHALLERS(TInt8) 

    DECLARE_MARSHALLERS(TUChar) 

        //IMPL_MARSHALLERS(TUInt8) 

    DECLARE_MARSHALLERS(TInt16) 

    DECLARE_MARSHALLERS(TUInt16) 

    DECLARE_MARSHALLERS(TInt32) 

    DECLARE_MARSHALLERS(TUInt32) 

    DECLARE_MARSHALLERS(TFloat) 

    DECLARE_MARSHALLERS(TDouble) 

    DECLARE_MARSHALLERS(TInt64) 

    DECLARE_MARSHALLERS(TUInt64) 

    //DECLARE_MARSHALLERS(TBOOL)  
#else

// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TUChar ucChar);
// 
 TInt32 GetLength(const TUChar c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TChar ucChar);
// 
 TInt32 GetLength(const TChar c);
// // inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TInt8 nInt8);
// // inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,const TInt8& nInt8);
// // 
// // inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TUInt8 uInt8);
// // inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,const TUInt8& uInt8);
// 
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TInt16 nInt16);
// 
 TInt32 GetLength(const TInt16 c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TUInt16 uInt16);
// 
 TInt32 GetLength(const TUInt16 c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TInt32 nInt32);
// 
 TInt32 GetLength(const TInt32 c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TUInt32 uInt32);
// 
 TInt32 GetLength(const TUInt32 c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TFloat fFloat);
// 
 TInt32 GetLength(const TFloat c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TDouble dDouble);
// 
 TInt32 GetLength(const TDouble c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TInt64 llInt64);
// 
 TInt32 GetLength(const TInt64 c);
// 
 TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TUInt64 ullInt64);
// 
 TInt32 GetLength(const TUInt64 c);
// 
// //inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TBOOL bBool); //为什么会和TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,CDoid *pDoid)冲突呢？！
// 
 TInt32 GetLength(const TBOOL c);
#endif

TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TChar *psz);
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TChar *psz);

TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const string &psz);
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,string &psz);

template<class TYPE>
TInt32 GetLength(const OctSeq<TYPE> &tOctSeq)
{
    return tOctSeq.GetFullLength();
}

template<class TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const OctSeq<TYPE> &tOctSeq)
{
    TInt32 nLength = tOctSeq.GetFullLength();
    if (nLength > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    *((TYPE*)pBuffer) = tOctSeq.m_nBodyLength;
    if (tOctSeq.m_nBodyLength > 0)
    {
        memcpy((pBuffer+sizeof(TYPE)),tOctSeq.m_pBuffer,tOctSeq.m_nBodyLength);
    }
    return nLength;
}

template<class LENGTH_TYPE,class TAG_TYPE>
TInt32 GetLength(const TLV<LENGTH_TYPE,TAG_TYPE> &tTLV)
{
    return tTLV.GetFullLength();
}


template<class LENGTH_TYPE,class TAG_TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TLV<LENGTH_TYPE,TAG_TYPE> &tTLV)
{
    TInt32 nLength = tTLV.GetFullLength();
    if (nLength > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    *((LENGTH_TYPE*)pBuffer) = tTLV.m_nBodyLength;
    pBuffer += sizeof(LENGTH_TYPE);
    *((TAG_TYPE*)pBuffer) = tTLV.m_nTag;
    pBuffer += sizeof(TAG_TYPE);
    if (tTLV.m_nBodyLength > 0)
    {
        memcpy(pBuffer,tTLV.m_pBuffer,tTLV.m_nBodyLength);
    }
    return nLength;
}

}



template<class TYPE>
TInt32 GetLength(const vector<TYPE> &tVector)
{
    TInt32 nLen = tVector.size();
    if (nLen)
    {
        TInt32 nNeed = sizeof(TInt32);
        for(TInt32 i=0;i<nLen;++i)
        {
            nNeed += GetLength(tVector[i]);
        }
        return nNeed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const vector<TYPE> &tVector)
{
    TInt32 nLen = tVector.size();
    *((TInt32*)pBuffer) = nLen;
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);
        for(TInt32 i=0;i<nLen;++i)
        {
            TInt32 nRet = Marshall(pBuffer+nUsed,uBuffLen-nUsed,tVector[i]);
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
TInt32 GetLength(const list<TYPE> &tList)
{
    TInt32 nLen = tList.size();
    if (nLen)
    {
        TInt32 nNeed = sizeof(TInt32);
        list<TYPE>::const_iterator it = tList.begin();
        list<TYPE>::const_iterator iEnd = tList.end();
        while(it != iEnd)
        {
            nNeed += GetLength(*it);
            ++it;
        }
        return nNeed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const list<TYPE> &tList)
{
    TInt32 nLen = tList.size();
    *((TInt32*)pBuffer) = nLen;
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);
        list<TYPE>::const_iterator it = tList.begin();
        list<TYPE>::const_iterator iEnd = tList.end();
        while(it != iEnd)
        {
            TInt32 nRet = Marshall(pBuffer+nUsed,(uBuffLen-nUsed),*it);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            ++it;
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class TYPE>
TInt32 GetLength(const set<TYPE> &tSet)
{
    TInt32 nLen = tSet.size();
    if (nLen)
    {
        TInt32 nNeed = sizeof(TInt32);
        set<TYPE>::const_iterator it = tSet.begin();
        set<TYPE>::const_iterator iEnd = tSet.end();
        while(it != iEnd)
        {
            nNeed += GetLength(*it);
            ++it;
        }
        return nNeed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const set<TYPE> &tSet)
{
    TInt32 nLen = tSet.size();
    *((TInt32*)pBuffer) = nLen;
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);
        set<TYPE>::const_iterator it = tSet.begin();
        set<TYPE>::const_iterator iEnd = tSet.end();
        while(it != iEnd)
        {
            TInt32 nRet = Marshall(pBuffer+nUsed,(uBuffLen-nUsed),*it);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            ++it;
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class CKEY,class TYPE>
TInt32 GetLength(const map<CKEY,TYPE> &tMap)
{
    TInt32 nLen = tMap.size();
    if (nLen)
    {
        TInt32 nNeed = sizeof(TInt32);
        map<CKEY,TYPE>::const_iterator it = tMap.begin();
        map<CKEY,TYPE>::const_iterator iEnd = tMap.end();
        while(it != iEnd)
        {
            nNeed += GetLength(it->first);
            nNeed += GetLength(it->second);
            ++it;
        }
        return nNeed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

template<class CKEY,class TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const map<CKEY,TYPE> &tMap)
{
    TInt32 nLen = tMap.size();
    *((TInt32*)pBuffer) = nLen;
    if (nLen)
    {
        TInt32 nUsed = sizeof(TInt32);
        map<CKEY,TYPE>::const_iterator it = tMap.begin();
        map<CKEY,TYPE>::const_iterator iEnd = tMap.end();
        while(it != iEnd)
        {
            TInt32 nRet = Marshall(pBuffer+nUsed,(uBuffLen-nUsed),it->first);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            nRet = Marshall(pBuffer+nUsed,(uBuffLen-nUsed),it->second);
            if (nRet < SUCCESS)
            {
                return nRet;
            }
            nUsed += nRet;
            ++it;
        }
        return nUsed;
    }
    else
    {
        return sizeof(TInt32);
    }
}

#endif

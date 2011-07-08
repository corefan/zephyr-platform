#ifndef __ZEPHYR_PUBLIC_TYPE_MARSHALLER_H__
#define __ZEPHYR_PUBLIC_TYPE_MARSHALLER_H__

#include "TypeDef.h"

namespace Zephyr
{

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TUChar ucChar);

inline TInt32 GetLength(TUChar c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TChar ucChar);

inline TInt32 GetLength(TChar c);
// inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt8 nInt8);
// inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt8& nInt8);
// 
// inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt8 uInt8);
// inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt8& uInt8);


inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt16 nInt16);

inline TInt32 GetLength(TInt16 c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt16 uInt16);

inline TInt32 GetLength(TUInt16 c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt32 nInt32);

inline TInt32 GetLength(TInt32 c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt32 uInt32);

inline TInt32 GetLength(TUInt32 c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TFloat fFloat);

inline TInt32 GetLength(TFloat c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TDouble dDouble);

inline TInt32 GetLength(TDouble c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt64 llInt64);

inline TInt32 GetLength(TInt64 c);

inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt64 ullInt64);

inline TInt32 GetLength(TInt64 c);

//inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TBOOL bBool); 为什么会和TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,CDoid *pDoid)冲突呢？！

inline TInt32 GetLength(TBOOL c);

template<class TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,OctSeq<TYPE> &tOctSeq)
{
    TInt32 nLength = tOctSeq.GetFullLength();
    if (nLength > uBuffLen)
    {
        return OUT_OF_MEM;
    }
    *((TYPE*)pBuffer) = tOctSeq.m_nBodyLength;
    memcpy((pBuffer+sizeof(TYPE)),tOctSeq.m_pBuffer,tOctSeq.m_nBodyLength);
    return nLength;
}

template<class LENGTH_TYPE,class TAG_TYPE>
TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TLV<LENGTH_TYPE,TAG_TYPE> &tTLV)
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
    memcpy(pBuffer,tTLV.m_pBuffer,tTLV.m_nBodyLength);
    return nLength;
}
}



#endif

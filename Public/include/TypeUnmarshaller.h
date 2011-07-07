#ifndef __ZEPHYR_PUBLIC_TYPE_UNMARSHALLER_H__
#define __ZEPHYR_PUBLIC_TYPE_UNMARSHALLER_H__

#include "TypeDef.h"

namespace Zephyr
{

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUChar& ucChar);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUChar& ucChar);


    // inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TInt8 nInt8);
    // inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt8& nInt8);
    // 
    // inline TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt8 uInt8);
    // inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt8& uInt8);


    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt16& nInt16);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt16& uInt16);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt32& nInt32);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TUInt32& uInt32);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TFloat& fFloat);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TDouble& dDouble);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt64& llInt64);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TInt64& ullInt64);

    inline TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TBool& bBool);

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



#endif

#include "../include/TypeMarshaller.h"
#include "../include/SysMacros.h"
#include <string.h>

namespace Zephyr
{
#ifndef _USE_LINK_2_MARSHALL
 
#else

#define IMPL_MARSHALLERS(TYPE) \
    TInt32 Marshall(TUChar *pBuffer,TInt32 uBufferLen,TYPE tType) \
    { \
    if (uBufferLen >= sizeof( TYPE )) \
    { \
    *(( TYPE *)pBuffer) = tType; \
    return sizeof( TYPE ); \
} \
    return OUT_OF_MEM; \
} \
    TInt32 GetLength(TYPE c) \
    { \
    return sizeof(TYPE); \
}
    // 

    IMPL_MARSHALLERS(TChar) 

        //IMPL_MARSHALLERS(TInt8) 

        IMPL_MARSHALLERS(TUChar) 

        //IMPL_MARSHALLERS(TUInt8) 

        IMPL_MARSHALLERS(TInt16) 

        IMPL_MARSHALLERS(TUInt16) 

        IMPL_MARSHALLERS(TInt32) 

        IMPL_MARSHALLERS(TUInt32) 

        IMPL_MARSHALLERS(TFloat) 

        IMPL_MARSHALLERS(TDouble) 

        IMPL_MARSHALLERS(TInt64) 

        IMPL_MARSHALLERS(TUInt64) 

        IMPL_MARSHALLERS(TBOOL)  
#endif
#pragma warning(push)
#pragma warning(disable:4267)


TInt32 GetLength(TChar *psz)
{
    TInt32 n = strlen(psz);
    return (n+(sizeof(TInt32)+sizeof(TChar)));
}


TInt32 GetLength(const TChar *psz)
{
     TInt32 n = strlen(psz);
     return (n+(sizeof(TInt32)+sizeof(TChar)));
}

TInt32 Marshall(TUChar *pBuffer,TInt32 uBuffLen,const TChar *psz)
{
    if (NULL == psz)
    {
        psz = "";
    }
    if (uBuffLen < (sizeof(TInt32)+sizeof(TChar)))
    {
        return OUT_OF_MEM_BUFFER; 
    }
    uBuffLen -= (sizeof(TInt32)+sizeof(TChar));
    TInt32 n = strnlen(psz,uBuffLen);
    if (n+(sizeof(TInt32)+sizeof(TChar)) < uBuffLen)
    {
        return OUT_OF_MEM_BUFFER;
    }
    *((TInt32*)pBuffer) = n;
    memcpy((char*)(pBuffer+sizeof(TInt32)),psz,(n));
    pBuffer[n+(sizeof(TInt32)+sizeof(TChar))] = 0;//
    return n + (sizeof(TInt32)+sizeof(TChar));
}
#pragma warning(pop)

}

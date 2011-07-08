#include "../include/TypeMarshaller.h"
#include "../include/SysMacros.h"

namespace Zephyr
{

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

}

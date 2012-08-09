#include "../include/TypeUnmarshaller.h"
#include "../include/SysMacros.h"
#include <string.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable:4267)
#pragma warning(disable:4018)

namespace Zephyr
{
#ifdef _USE_LINK_2_MARSHALL
#define IMPL_UNMARSHALLERS(TYPE) \
TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBufferLen,TYPE &tType) \
{ \
    if (uBufferLen >= sizeof(TYPE)) \
    { \
        tType = *((TYPE*)pBuffer); \
        return sizeof(TYPE); \
    } \
    return OUT_OF_MEM; \
}

// 
 IMPL_UNMARSHALLERS(TChar) 
 
  //IMPL_MARSHALLERS(TInt8) 
 
 IMPL_UNMARSHALLERS(TUChar) 
 
  //IMPL_MARSHALLERS(TUInt8) 
 
 IMPL_UNMARSHALLERS(TInt16) 
 
 IMPL_UNMARSHALLERS(TUInt16) 
 
 IMPL_UNMARSHALLERS(TInt32) 
 
 IMPL_UNMARSHALLERS(TUInt32) 
 
 IMPL_UNMARSHALLERS(TFloat) 
 
 IMPL_UNMARSHALLERS(TDouble) 
 
 IMPL_UNMARSHALLERS(TInt64) 
 
 IMPL_UNMARSHALLERS(TUInt64) 
 
 IMPL_UNMARSHALLERS(TBOOL) 
#endif



TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,TChar *&psz)
{
    if (uBuffLen < (sizeof(TUInt32)+sizeof(TChar)))
    {
        return OUT_OF_RANGE;
    }
    TInt32 n = *((TInt32*)pBuffer);
    uBuffLen -= sizeof(TInt32);
    if (uBuffLen < (n+sizeof(TChar))) //must larger than it.!
    {
#ifdef _DEBUG
        assert(0);
#endif
        return OUT_OF_RANGE;
    }
    pBuffer[n+sizeof(TInt32)] = 0; //force add '\0';
    psz = (TChar *)(pBuffer+sizeof(TInt32));
    return (n + ((TUInt32)+sizeof(TChar)));
}

TInt32 Unmarshall(TUChar *pBuffer,TInt32 uBuffLen,const TChar *&psz)
{
    if (uBuffLen < (sizeof(TUInt32)+sizeof(TChar)))
    {
        return OUT_OF_RANGE;
    }
    TInt32 n = *((TInt32*)pBuffer);
    uBuffLen -= sizeof(TInt32);
    if (uBuffLen < (n+sizeof(TChar))) //must larger than it.!
    {
#ifdef _DEBUG
        assert(0);
#endif
        return OUT_OF_RANGE;
    }
    pBuffer[n+sizeof(TInt32)] = 0; //force add '\0';
    psz = (TChar *)(pBuffer+sizeof(TInt32));
    return (n + ((TUInt32)+sizeof(TChar)));
}


#pragma warning(pop)
}

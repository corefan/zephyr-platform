#ifndef __ZEPHYR_IDL_GENERATOR_PARAMETER_H__
#define __ZEPHYR_IDL_GENERATOR_PARAMETER_H__

#include "./baseElement.h"
#include "./FullTypeClass.h"
namespace Zephyr
{


class CParamerter : public CBaseElement
{
public:
    DECLARE_STATIC_CLASS_POOL(CParamerter);
public:
    CFullTypeDef *m_pFullType;

    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);
};
}

#endif

#include "../include/baseElement.h"

namespace Zephyr
{

TInt32 CBaseElement::IgnorType(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles,EnType eType)
{
    TInt32 nRet = 0;
    TInt32 total = nTotalEles - nProcess2;
    while (nRet < total)
    {
        EnType e = pTypes[nProcess2+nRet];
        if (e != eType)
        {
            return nRet;
        }
        ++nRet;
    }
    return nRet;
}

TInt32 CBaseElement::Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    return 0;
}

TInt32 CBaseElement::IgnorTypes(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles,int nrOfType,EnType eIgnoreTypes[])
{
    TInt32 nRet = 0;
    TInt32 total = nTotalEles - nProcess2;
    while (nRet < total)
    {
        EnType e = pTypes[nProcess2+nRet];
        bool bNotIgnore = true;
        for (int i=0;i<nrOfType;++i)
        {
            if (e==eIgnoreTypes[i])
            {
                bNotIgnore = false;
                break;
            }
        }
        if (bNotIgnore)
        {
            return nRet;;
        }
        ++nRet;
    }
    return nRet;
}

}
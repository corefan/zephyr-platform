#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/IdlGeneratorErrorCode.h"
namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CMethod);

TInt32 CMethod::Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    if (blanket_type_1 == pTypes[nProcess2])
    {
        ++nProcess2;
        while(blanket_type_2 != pTypes[nProcess2])
        {
            CParamerter *pParameter = CREATE_FROM_STATIC_POOL(CParamerter);
            if(!pParameter)
            {
                OnError(nProcess2);
                return OUT_OF_MEM;
            }
            int nRet = pParameter->Process(ppElements,pTypes,nProcess2,nTotalEles);
            if (nRet > SUCCESS)
            {
                nProcess2 += nRet;
                AddChildElement(pParameter);
            }
            else
            {
                OnError(nProcess2);
                return nRet;
            }
        }
    }
    return TYPE_NOT_FOUND;
}

}
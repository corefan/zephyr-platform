#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/IdlGeneratorErrorCode.h"
namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CMethod);

TInt32 CMethod::Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    int nOld = nProcess2;
    if (blanket_type_1 == pTypes[nProcess2])
    {
        ++nProcess2;
        int nNrOfParameter = 0;
        while((nProcess2<nTotalEles)&&(blanket_type_2 != pTypes[nProcess2]))
        {
            if (nNrOfParameter)
            {
                bool bCorrect = false;
                if (operator_type == pTypes[nProcess2])
                {
                    if (0 == strcmp(",",ppElements[nProcess2]))
                    {
                        bCorrect = true;
                    }
                }
                if (bCorrect)
                {
                    ++nProcess2;
                }
                else
                {
                    OnError(ppElements[nProcess2]);
                    return -1;
                }
            }
            CParamerter *pParameter = CREATE_FROM_STATIC_POOL(CParamerter);
            if(!pParameter)
            {
                char *pAt =NULL;
                if (nProcess2 < nTotalEles)
                {
                    pAt = ppElements[nTotalEles];
                }
                OnError(pAt);
                return OUT_OF_MEM;
            }
            int nRet = pParameter->Process(ppElements,pTypes,nProcess2,nTotalEles);
            if (nRet > SUCCESS)
            {
                nProcess2 += nRet;
                ++nNrOfParameter;
                AddChildElement(pParameter);
                m_szFullName += "_";
                m_szFullName += pParameter->m_pFullType->GetHierachyName();
            }
            else
            {
                char *pAt =NULL;
                if (nProcess2 < nTotalEles)
                {
                    pAt = ppElements[nTotalEles];
                }
                OnError(pAt);
                return nRet;
            }
            
        }
        ++nProcess2;
    }
    //»¹ÓÐ ¡°= 0¡±
    if (nProcess2 < nTotalEles)
    {
        if (operator_type == pTypes[nProcess2])
        {
            if (0 == strcmp("=",ppElements[nProcess2]))
            {
                ++nProcess2;
                if (num_type == pTypes[nProcess2])
                {
                    if(0 == strcmp("0",ppElements[nProcess2]))
                    {
                        ++nProcess2;
                        ++m_nIsVirtual;
                    }
                }
            }
        }
        else
        {
            OnError(ppElements[nProcess2]);
            return -1;
        }
    }
    if (nProcess2 < nTotalEles)
    {
        if (semicolon_type == pTypes[nProcess2])
        {
            ++nProcess2;
            return (nProcess2 - nOld);
        }
        OnError(ppElements[nProcess2]);
    }
    OnError("incorrect end!");
    return -1;
}

const char *CMethod::GetHierachyName()
{
    return m_szFullName.c_str();
}

}
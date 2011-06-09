#include "../include/FullTypeClass.h"

namespace Zephyr
{

    
IMPLEMENT_STATIC_CLASS_POOL(CFullTypeDef)

void CFullTypeDef::AddPrefix(EnPrefix enPrefix)
{
    if (!(m_uPrefix & enPrefix))
    {
        m_uPrefix |= enPrefix;
        switch (enPrefix)
        {
        case en_const_prefix:
            {
                m_szFull += "const ";
            }
            break;
        case en_static_prefix:
            {
                m_szFull += "static ";
            }
            break;
        case en_mutable_prefix:
            {
                m_szFull += "mutable ";
            }
            break;
        case en_volatile_prefix:
            {
                m_szFull += "volatile ";
            }
            break;
        }
    }
}


CFullTypeDef::CFullTypeDef()
{
    m_uPrefix = 0;
    m_pType = NULL;
    m_szFull.clear();
    m_nElmentType = raw_full_type_type;
    for (int i=0;i<4;++i)
    {
        m_tOprs[i] = 0;
    }
}

void CFullTypeDef::AddOpr(EnOperator enOpr)
{
    for (int i=0;i<4;++i)
    {
        if (en_no_opr == m_tOprs[i])
        {
            switch (enOpr)
            {
            case en_star_operator: 
                {
                    m_szFull += "* ";
                    m_tOprs[i] = en_star_operator;
                }
                break;
            case en_ref_operator:
                {
                    m_szFull += "& ";
                    m_tOprs[i] = en_ref_operator;
                }
                break;
            }
            break;
        }
    }
}

TInt32 CFullTypeDef::Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    int nOld = nProcess2;
    CBaseElement *pEle = IsKeyWords(ppElements[nProcess2]);
    while (pEle)
    {
        switch (pEle->m_nElmentType)
        {
        case key_static:
            {
                AddPrefix(en_static_prefix);
                ++nProcess2;
            }
            break;
        case key_const:
            {
                AddPrefix(en_const_prefix);
                ++nProcess2;
            }
            break;
        case key_volatile:
            {
                AddPrefix(en_volatile_prefix);
                ++nProcess2;
            }
            break;
        case key_mutable:
            {
                AddPrefix(en_mutable_prefix);
                ++nProcess2;       
            }
            break;
        default:
            {
                
            }
        }
        pEle = IsKeyWords(ppElements[nProcess2]);
    }
    
    //Ò»¸ötypename
    if (alphabet_type == pTypes[nProcess2])
    {
        //find a type
        CBaseElement *pFoundType = IsOneType(ppElements[nProcess2]);
        if (!pFoundType)
        {
            pFoundType = CREATE_FROM_STATIC_POOL(CBaseElement);
            if (!pFoundType)
            {
                printf("Out of Mem");
                
                return OUT_OF_MEM;
            }
            pFoundType->SetName(ppElements[nProcess2]);
            AddType(pFoundType);
        }
        m_pType = pFoundType;
        m_szFull += ppElements[nProcess2];
        ++nProcess2;
    }
    while((operator_type == pTypes[nProcess2])||(star_mark_type == pTypes[nProcess2]))
    {
        int n = 0;
        while(0 != ppElements[nProcess2][n])
        {
            switch(ppElements[nProcess2][n])
            {
            case '*':
                {
                    AddOpr(en_star_operator);
                }
                break;
            case '&':
                {
                    AddOpr(en_ref_operator);
                }
                break;
            default:
                {
                    char *pAt =NULL;
                    if (nProcess2 < nTotalEles)
                    {
                        pAt = ppElements[nTotalEles];
                    }
                    OnError(pAt);
                    return -1;
                }
            }
            ++n;
        }
        ++nProcess2;
    }
    return (nProcess2 - nOld);
}

}
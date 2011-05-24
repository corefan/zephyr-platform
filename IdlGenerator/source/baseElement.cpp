#include "../include/baseElement.h"

namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CBaseElement)



TplMap<TplPtPack<CBaseElement,string >,string> *CBaseElement::sm_pBaseElements = NULL;
TplMap<TplPtPack<CBaseElement,string >,string> *CBaseElement::sm_pBaseKeyWords = NULL;

TInt32   CBaseElement::AddType(CBaseElement *pBaseElement)
{
    TplPtPack<CBaseElement,string > *pItem = sm_pBaseElements->PrepareItem();
    if (pItem)
    {
        (*pItem).m_pPt = pBaseElement;
        int nRet = sm_pBaseElements->AddInTree(pItem);
        if (nRet < SUCCESS)
        {
            sm_pBaseElements->ReleaseItem(pItem);
            return nRet;
        }
        return SUCCESS;
    }
    return OUT_OF_MEM;
}
TInt32   CBaseElement::AddKeyWords(const char *pName,EnKeyWords key)
{
    TplPtPack<CBaseElement,string > *pItem = sm_pBaseKeyWords->PrepareItem();
    if (pItem)
    {
        CBaseElement *pElem = CREATE_FROM_STATIC_POOL(CBaseElement);
        pElem->m_szName = pName;
        pElem->m_nElmentType = key;
        (*pItem).m_pPt = pElem;
        int nRet = sm_pBaseKeyWords->AddInTree(pItem);
        if (nRet < SUCCESS)
        {
            RELEASE_INTO_STATIC_POOL(CBaseElement,pElem);
            return nRet;
        }
        return SUCCESS;
    }
    return OUT_OF_MEM;
}

CBaseElement *CBaseElement::IsKeyWords(const char *psz)
{
    string str = psz;
    TplPtPack<CBaseElement,string > *pElePack = sm_pBaseKeyWords->GetItemByKey(&str);
    if (pElePack)
    {
        return pElePack->m_pPt;
    }
    return NULL;
}




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

const char *CBaseElement::GetHierachyName()
{
    return m_szName.c_str();
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

void CBaseElement::AddChildElement(CBaseElement *pElement,const char *pSubType,const char* pszName)
{
    CInstance t;
    t.m_tType.m_pPt = pElement;
    t.m_szInstanceName = pszName;
    strncpy(t.m_nSubType,pSubType,4);
    t.m_nSubType[3] = 0;
    m_tChilds.push_back(t);
}

}
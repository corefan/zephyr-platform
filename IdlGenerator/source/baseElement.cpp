#include "../include/baseElement.h"
#pragma warning(push)
#pragma warning(disable:4267)

namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CBaseElement)


TInt32 CBaseElement::sm_nInterfaceIdBegin = 0;

TplMap<TplPtPack<CBaseElement,string >,string> *CBaseElement::sm_pBaseElements = NULL;
TplMap<TplPtPack<CBaseElement,string >,string> *CBaseElement::sm_pBaseKeyWords = NULL;
std::map<string,string> CBaseElement::sm_tCType2CSharp;
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

TInt32   CBaseElement::AddBaseType(const char *psz,EnRawTypes nType)
{
    CBaseElement *pBase = CREATE_FROM_STATIC_POOL(CBaseElement);
    if (!pBase)
    {
        return OUT_OF_MEM;
    }
    pBase->SetName(psz);
    pBase->m_nElmentType = nType;
    return AddType(pBase);
}

CBaseElement *CBaseElement::IsOneType(const char *psz)
{
    string key = psz;
    TplPtPack<CBaseElement,string > *pElePack = sm_pBaseElements->GetItemByKey(key);
    if (pElePack)
    {
        return pElePack->m_pPt;
    }
    return NULL;
}

TInt32   CBaseElement::AddKeyWords(const char *pName,EnRawTypes key)
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
    TplPtPack<CBaseElement,string > *pElePack = sm_pBaseKeyWords->GetItemByKey(str);
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

TInt32 CBaseElement:: Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    return 0;
}

const char *CBaseElement::GetHierachyName()
{
    return m_szName.c_str();
}

void CBaseElement::MakeOneWords(char **ppWords,int nFrom,int nWordsNr)
{
    if (nWordsNr>0)
    {
        --nWordsNr;
        for (int i=1;i<nWordsNr;++i)
        {
            int nLen = strlen(ppWords[nFrom+i]);
            memmove(ppWords[nFrom+i]-i,ppWords[nFrom+i],nLen);
        }
        int nLen = strlen(ppWords[nFrom+nWordsNr])+1;
        memmove(ppWords[nFrom+nWordsNr]-nWordsNr,ppWords[nFrom+nWordsNr],nLen);
    }
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

void CBaseElement::AddChildElement(CBaseElement *pElement)
{
    pElement->m_pFather = this;
    TplPtPack<CBaseElement,string> t;
    t.m_pPt = pElement;
    m_tChilds.push_back(t);
}
CBaseElement *CBaseElement::GetLastElement()
{
    return m_tChilds[m_tChilds.size()-1].m_pPt;
}
bool CBaseElement::IsBracesBegin(char *pStr,EnType nType)
{
    if (operator_type == nType)
    {
        if (pStr[0] == '{')
        {
            return true;
        }
    }
    return false;
}

bool CBaseElement::IsBracesEnd(char *pStr,EnType nType)
{
    if (operator_type == nType)
    {
        if (pStr[0] == '}')
        {
            return true;
        }
    }
    return false;
}


void CBaseElement::OnError(const char *pWord)
{
    if (pWord)
    {
        printf("OnError at %s \n",pWord);
    }
    else
    {
        printf("OnError at end \n");
    }
}

TInt32 CBaseElement::GenerateSkeleton(const char *pPath)
{
    int n = m_tChilds.size();
    int nTotalSize = 0;
    for(int i=0;i<n;++i)
    {
        int nRet = m_tChilds[i]->GenerateSkeleton(pPath);
        if (nRet < SUCCESS)
        {
            nTotalSize += nRet;
        }
    }
    return nTotalSize;
}

TInt32 CBaseElement::GenerateStub(const char *pPath)
{
    int n = m_tChilds.size();
    int nTotalSize = 0;
    for(int i=0;i<n;++i)
    {
        int nRet = m_tChilds[i]->GenerateStub(pPath);
        if (nRet < SUCCESS)
        {
            nTotalSize += nRet;
        }
    }
    return nTotalSize;
}

TInt32 CBaseElement::GenerateMethodId(const char *pPath)
{
    int n = m_tChilds.size();
    int nTotalSize = 0;
    for(int i=0;i<n;++i)
    {
        int nRet = m_tChilds[i]->GenerateMethodId(pPath);
        if (nRet < SUCCESS)
        {
            nTotalSize += nRet;
        }
    }
    return nTotalSize;
}
TInt32 CBaseElement::WriteEtch(char *psz,int nLayers)
{
    int nUsed = 0;
    for (int k=0;k<nLayers;++k)
    {
        int n = sprintf(psz+nUsed,"    ");
        nUsed +=n;
    }
    return nUsed;
}

TInt32 CBaseElement::GenerateCSharpCode(const char *pPath)
{
    TInt32 nFileNr = 0;
    for(int i=0;i<m_tChilds.size();++i)
    {
        nFileNr += m_tChilds[i]->GenerateCSharpCode(pPath);
    }
    return SUCCESS;
}

void CBaseElement::AddCSharpType(const char *pOrigianl,const char* pNew)
{
    string szOrig = pOrigianl;
    sm_tCType2CSharp[szOrig] = pNew;
}

string *CBaseElement::GetCSharpType(const char *pCType)
{
    map<string,string>::iterator it = sm_tCType2CSharp.find(pCType);
    if (it != sm_tCType2CSharp.end())
    {
        return &it->second;
    }
    return NULL;
}


#define ADD_TYPE_MAP(C_TYPE,CS_TYPE) AddCSharpType(#C_TYPE,#CS_TYPE); \
                                     AddCSharpType(#C_TYPE"*",#CS_TYPE); \
                                     AddCSharpType(#C_TYPE" *",#CS_TYPE);\

void CBaseElement::InitTypeMap()
{
    ADD_TYPE_MAP(TChar,char);
    ADD_TYPE_MAP(TUInt8,byte);
    ADD_TYPE_MAP(TInt8,sbyte);
    ADD_TYPE_MAP(TInt16,short);
    ADD_TYPE_MAP(TUInt16,ushort);
    ADD_TYPE_MAP(TUInt32,uint);
    ADD_TYPE_MAP(TInt32,int);
    ADD_TYPE_MAP(TInt64,long);
    ADD_TYPE_MAP(TUInt64,ulong);
    ADD_TYPE_MAP(TFloat,float);
    ADD_TYPE_MAP(TDouble,double);
    ADD_TYPE_MAP(double,double);
    ADD_TYPE_MAP(char,char);
    ADD_TYPE_MAP(int,int);
    ADD_TYPE_MAP(unsigned int,uin);
    ADD_TYPE_MAP(short,short);
    ADD_TYPE_MAP(unsigned short,ushort);
    ADD_TYPE_MAP(longlong,long);
    ADD_TYPE_MAP(unsigned longlong,ulong);
    ADD_TYPE_MAP(float,float);
    ADD_TYPE_MAP(string,string);
}

}
#pragma warning(pop)

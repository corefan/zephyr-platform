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

int CBaseElement::WriteCSharpCode(const TChar *pPath)
{
    std::string szFileName = pPath;
    int nPathLen = szFileName.size();
    if (szFileName[nPathLen-1]=='/')
    {
    }
    else
    {
        szFileName +="/";
    }
    szFileName += "BaseTypeMarshaller.cs";
    FILE *pFile = fopen(szFileName.c_str(),"w");
    int nLength = 2*1024*1024;
    char *pBuff = NULL;
    NEW(pBuff,char,nLength);
    if (!pBuff)
    {
        return OUT_OF_MEM;
    }
    int nUsed = 0;

    char *pBaseTypes[12] = {"byte","sbyte","char","short","ushort","int","uint","long","ulong","float","double","string"};
    for (int i=0;i<12;++i)
    {
        int n = GenerateCommonTypeMarshallerCSharpCode(pBuff+nUsed,pBaseTypes[i],True);
        nUsed += n;
    }

    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);

    szFileName = pPath;
    nPathLen = szFileName.size();
    if (szFileName[nPathLen-1]=='/')
    {
    }
    else
    {
        szFileName +="/";
    }
    szFileName += "BaseTypeUnMarshaller.cs";
    pFile = fopen(szFileName.c_str(),"w");

    nUsed = 0;
    for (int i=0;i<12;++i)
    {
        int n = GenerateCommonTypeUnMarshallerCSharpCode(pBuff+nUsed,pBaseTypes[i]);
        nUsed += n;
    }

    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);
    delete [] pBuff;
    pBuff = NULL;
    return nUsed;
}

int CBaseElement::GenerateCommonTypeMarshallerCSharpCode(TChar *pBuff,const TChar *pszCommonType,TBOOL bBasicType)
{
    int nLength = 2*1024*1024;
    int nUsed = 0;
    int n = 0;
    int nEtchNr=1;
    WRITE_LINE_ETCH("static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<%s> tValue)",pszCommonType);
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    const TChar *pNeedRef="";
    if (!bBasicType)
    {
        pNeedRef = "ref ";
    }
    WRITE_LINE_ETCH("if ((nUsed + GetLength(%stValue)) > nBuferLength)",pNeedRef);
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("return MacrosAndDef.NOT_ENOUGH_BUFFER;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);");
    WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("return nRet;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("int nMarshalled = nRet;");
    WRITE_LINE_ETCH("foreach(%s item in tValue)",pszCommonType);
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("nRet = Marshall(pBuffers, nBuferLength, nUsed+nMarshalled,%s item);",pNeedRef);
    WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("return nRet;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("nMarshalled += nRet;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("return nMarshalled;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
 

    WRITE_LINE_ETCH("static public int GetLength(List<%s> tValue)",pszCommonType);
    WRITE_LINE_ETCH("{");
    WRITE_LINE_ETCH("int nLen = sizeof(int);");
    WRITE_LINE_ETCH("foreach (%s item in tValue)",pszCommonType);
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("nLen += GetLength(%sitem);",pNeedRef);
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("return nLen;");
    WRITE_LINE_ETCH("}");

    
    char *szTypes[4]={"uint","int","ulong","long"};
    for (int i=0;i<4;++i)
    {
        WRITE_LINE_ETCH("static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<%s,%s> tValue)",szTypes[i],pszCommonType);
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("if ((nUsed + GetLength(tValue)) > nBuferLength)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return MacrosAndDef.NOT_ENOUGH_BUFFER;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);");
        WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("int nMarshalled = nRet;");
        WRITE_LINE_ETCH("foreach (KeyValuePair<%s, %s> item in tValue)",szTypes[i],pszCommonType);
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Key);");
        WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("nMarshalled += nRet;");
        WRITE_LINE_ETCH("nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled,%s item.Value);",pNeedRef);
        WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("nMarshalled += nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("return nMarshalled;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");

        WRITE_LINE_ETCH("static public int GetLength(Dictionary<%s,%s> tValue)",szTypes[i],pszCommonType);
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("int nLen = sizeof(int);");
        WRITE_LINE_ETCH("foreach (KeyValuePair<%s,%s> item in tValue)",szTypes[i],pszCommonType);
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("nLen += TypeMarshaller.GetLength(item.Key);");
        WRITE_LINE_ETCH("nLen += GetLength(%sitem.Value);",pNeedRef);
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("return nLen;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
    }
    return nUsed;
}

int CBaseElement::GenerateCommonTypeUnMarshallerCSharpCode(TChar *pBuff,const TChar *pszCommonType)
{
    int nLength = 2*1024*1024;
    int nUsed = 0;
    int n = 0;
    int nEtchNr=1;
    WRITE_LINE_ETCH("static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<%s> tValue)",pszCommonType);
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("tValue = new List<%s>();",pszCommonType);
    WRITE_LINE_ETCH("int nLen = 0;");
    WRITE_LINE_ETCH("int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);");
    WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("return nRet;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("int nUnmarshalled = nRet;");
    WRITE_LINE_ETCH("for (int i = 0; i < nLen; ++i)");
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("%s tOut;",pszCommonType);
    WRITE_LINE_ETCH("nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);");
    WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("return nRet;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("nUnmarshalled += nRet;");
    WRITE_LINE_ETCH("tValue.Add(tOut);");
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    WRITE_LINE_ETCH("return nUnmarshalled;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");

    char *szTypes[4]={"uint","int","ulong","long"};
    for (int i=0;i<4;++i)
    {
        WRITE_LINE_ETCH("static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<%s,%s> tValue)",szTypes[i],pszCommonType);
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("tValue = new Dictionary<%s, %s>();",szTypes[i],pszCommonType);
        WRITE_LINE_ETCH("int nLen = 0;");
        WRITE_LINE_ETCH("int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);");
        WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("int nUnmarshalled = nRet;");
        WRITE_LINE_ETCH("for (int i = 0; i < nLen; ++i)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("%s uKey;",szTypes[i]);
        WRITE_LINE_ETCH("nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out uKey);")
        WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("%s tOut;",pszCommonType);
        WRITE_LINE_ETCH("nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);");
        WRITE_LINE_ETCH("if (nRet < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return nRet;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("nUnmarshalled += nRet;");
        WRITE_LINE_ETCH("tValue.Add(uKey, tOut);");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
        WRITE_LINE_ETCH("return nUnmarshalled;");
        --nEtchNr;
        WRITE_LINE_ETCH("}");
    }
    return nUsed;
}


int CBaseElement::ReplaceStr(char *pBuffer,const TChar *pszOrig,const TChar *pNew,char *pszNew)
{
    int nUsed(0);
    int nOrigUsed(0);
    const char *pFound = strstr(pBuffer,pszOrig);
    int nOrigLen = strlen(pszOrig);
    int nNewLen = strlen(pNew);
    if (pFound)
    {
        while(pFound)
        {
            int nLen = (pFound - pBuffer+nOrigUsed);
            memcpy(pszNew+nUsed,pBuffer+nOrigUsed,nLen);
            nUsed += nLen;
            nOrigUsed += nLen + nOrigLen;
            memcpy(pszNew+nUsed,pNew,nNewLen+1);
            nUsed += nNewLen;
            pFound = strstr(pBuffer+nOrigUsed,pszOrig);
        }
        int nRemain = strlen(pBuffer+nOrigUsed) + 1;
        memcpy(pszNew+nUsed,pBuffer+nOrigUsed,nRemain+1);
        nUsed += nRemain;
        strcpy(pBuffer,pszNew);
    }
    else
    {
        return strlen(pBuffer);
    }
    //delete [] pszNew;
    return nUsed;
}

int CBaseElement::Replace4CSharp(char *pBuff)
{
    char *pszNew = new char[2*1024*1024];
    int nUsed = ReplaceStr(pBuff,"map<","Dictionary<",pszNew);
    nUsed = ReplaceStr(pBuff,"vector<","List<",pszNew);
    nUsed = ReplaceStr(pBuff,"list<","list<",pszNew);
    nUsed = ReplaceStr(pBuff,"set<","list<",pszNew);
    nUsed = ReplaceStr(pBuff,"OctSeq<ushort>","OctSeqUS",pszNew);
    nUsed = ReplaceStr(pBuff,"OctSeq<short>","OctSeqS",pszNew);
    nUsed = ReplaceStr(pBuff,"OctSeq<uint>","OctSeqUI",pszNew);
    nUsed = ReplaceStr(pBuff,"OctSeq<int>","OctSeqI",pszNew);
    nUsed = ReplaceStr(pBuff,"OctSeq<long>","OctSeqL",pszNew);
    nUsed = ReplaceStr(pBuff,"OctSeq<long>","OctSeqUL",pszNew);

    nUsed = ReplaceStr(pBuff,"TLV<ushort,ushort>","OctSeqUSUS",pszNew);
    nUsed = ReplaceStr(pBuff,"TLV<short,short>","OctSeqSS",pszNew);
    nUsed = ReplaceStr(pBuff,"TLV<int,int>","OctSeqII",pszNew);
    nUsed = ReplaceStr(pBuff,"TLV<uint,uint>","OctSeqUIUI",pszNew);
    nUsed = ReplaceStr(pBuff,"TLV<long,uint>","OctSeqLUI",pszNew);
    nUsed = ReplaceStr(pBuff,"TLV<ulong,uint>","OctSeqULUI",pszNew);
    delete [] pszNew;
    return nUsed;
}


#define ADD_TYPE_MAP(C_TYPE,CS_TYPE) AddCSharpType(#C_TYPE,#CS_TYPE); \
                                     AddCSharpType(#C_TYPE"*",#CS_TYPE); \
                                     AddCSharpType(#C_TYPE" *",#CS_TYPE);\

void CBaseElement::InitTypeMap()
{
    //ADD_TYPE_MAP(TChar,char);
    AddCSharpType("TChar","char");
    AddCSharpType("TChar*","string");
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
    //ADD_TYPE_MAP(char,char);
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

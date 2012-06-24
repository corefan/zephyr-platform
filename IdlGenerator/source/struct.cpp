#include "../include/Struct.h"
#include "../include/HeaderFile.h"
#include "../include/interfaceElement.h"
#include "../include/FullTypeClass.h"
#include "../include/IdlGeneratorErrorCode.h"
#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/nameSpace.h"

namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CStruct);


TInt32 CStruct::Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    EnState enLastState = en_class_key_word;
//     EnType enterAndDividerTypes[2] = {enter_type,divider_type};
//     struct Tt
//     {
//         char *m_pszTxt;
//         EnState   m_enType;
//     };
    vector<char*> pHeap;
    int nOld = nProcess2;
    
    while (nProcess2 < nTotalEles)
    {
        //忽律所有
        switch (enLastState)
        {
        case en_class_key_word:
            {
                if (pTypes[nProcess2] == alphabet_type)
                {
//                     m_szName.clear();
//                     if (m_pFather)
//                     {
//                         const char *pFathersName = m_pFather->GetName();
//                         if (pFathersName)
//                         {
//                             m_szName = pFathersName;
//                         }
//                     }
                    m_szName = ppElements[nProcess2];
                    enLastState = en_class_name;
                    ++nProcess2;
                }
                else
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
            break; //case en_class_key_word:
        case         en_class_name:        //CName
            {
                
                if (pTypes[nProcess2] == operator_type)
                {
                    if (ppElements[nProcess2][0] == ':')
                    {
                        ++nProcess2;
                        enLastState = en_class_name_risk;
                    }
                    else
                    {
                        if (pTypes[nProcess2] == operator_type)
                        {
                            if (ppElements[nProcess2][0] == ';') //只是个声明
                            {
                                AddType(this);
                                ++nProcess2;
                                return nProcess2;
                            }
                            else if (ppElements[nProcess2][0] == '{')
                            {
                                enLastState = en_class_left_brace;
                                ++nProcess2;
                            }
                            else
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
                        else
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
                }
                else
                {
                    char *pAt =NULL;
                    if (nProcess2 < nTotalEles)
                    {
                        pAt = ppElements[nTotalEles];
                    }
                    OnError(pAt);
                    return -1;
                }
            } //case         en_class_name: 
            break;
        case         en_class_name_risk:   //:
            {
                if (pTypes[nProcess2] == alphabet_type)
                {
                    if (0 == strcmp("virtual",ppElements[nProcess2]))
                    {
                        enLastState = en_class_derive_virtual;
                        ++nProcess2;
                    }
                    else
                    {
                        if (0 == strcmp("public",ppElements[nProcess2]))
                        {
                            ++nProcess2;
                        }
                        else if (0 == strcmp("protected",ppElements[nProcess2]))
                        {
                            ++nProcess2;
                        }
                        else if (0 == strcmp("private",ppElements[nProcess2]))
                        {
                            ++nProcess2;
                        }
                        if (pTypes[nProcess2] == alphabet_type)
                        {
                            enLastState = en_class_derive_class_name; //去掉名字
                            ++nProcess2;
                        }
                        else
                        {
                            printf("Incorrect line, a class name expected");
                            return -1;
                        }
                    }
                }
                else
                {
                    char *pAt =NULL;
                    if (nProcess2 < nTotalEles)
                    {
                        pAt = ppElements[nTotalEles];
                    }
                    OnError(pAt);
                    return -1;
                }
            } //case         en_class_name_risk:   //:
            break;
        case         en_class_derive_virtual://virtual 
            {
                if (pTypes[nProcess2] == alphabet_type)
                {
                    enLastState = en_class_derive_class_name;
                    ++nProcess2;
                }
                else
                {
                    char *pAt =NULL;
                    if (nProcess2 < nTotalEles)
                    {
                        pAt = ppElements[nTotalEles];
                    }
                    OnError(pAt);
                    return -1;
                }
            }  //case         en_class_derive_virtual://virtual 
            break;
        case         en_class_derive_class_name: //CDerive 
            {
                if (operator_type == pTypes[nProcess2])
                {
                    if (ppElements[nProcess2][0] == '{')
                    {
                        enLastState = en_class_left_brace;
                        ++nProcess2;
                    }
                    else
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
            } //
            break;
        case         en_class_left_brace:     //{ 这个时候需要的是一个type类型，或者 virtual \ pubic \ protected \private \ class \enum \struct \const \static \volatile \mutal 
            {
//                 int nRet = IgnorTypes(ppElements,pTypes,nProcess2,nTotalEles,2,enterAndDividerTypes); //
//                 nProcess2 += nRet;
                switch(pTypes[nProcess2])
                {
                case operator_type:
                    {
                        if (ppElements[nProcess2][0]=='}')//结束了
                        {
                            ++nProcess2;
//                             nRet = IgnorTypes(ppElements,pTypes,nProcess2,nTotalEles,2,enterAndDividerTypes); //
//                             nProcess2 += nRet;
                            if (semicolon_type == pTypes[nProcess2])
                            {
                                //if (ppElements[nProcess2][0]==';')//结束了
                                {
                                    ++nProcess2;
                                    return (nProcess2-nOld);
                                }
                            }
                            else
                            {
                                char *pAt =NULL;
                                if (nProcess2 < nTotalEles)
                                {
                                    pAt = ppElements[nTotalEles];
                                }
                                OnError(pAt);
                                return FAIL;
                            }
                        }
                        else if (ppElements[nProcess2][0]=='~')
                        {
                            //处理析构函数
                        }
                        else
                        {
                            char *pAt =NULL;
                            if (nProcess2 < nTotalEles)
                            {
                                pAt = ppElements[nTotalEles];
                            }
                            OnError(pAt);
                            return FAIL;
                        }
                       
                    }
                    break;
                case alphabet_type:
                    {
                        //if ()
                        CBaseElement *pEle = m_pOwner->IsKeyWords(ppElements[nProcess2]);
                        bool bNeedHandle = true;
                        if (pEle)
                        {
                            bNeedHandle = false;
                            switch (pEle->m_nElmentType)
                            {
							case key_using:
								{
									OnError(ppElements[nProcess2]);
									return INCORRECT_END;
								}
								break;
							case key_enum:
								{
									++nProcess2;
									while(semicolon_type != pTypes[nProcess2])
									{
                                        ++nProcess2;
										if (nProcess2 >= nTotalEles)
										{
											char *pAt = ppElements[(nProcess2-1)];
											OnError(pAt);
											return -1;
										}
									}
									++nProcess2;
								}
								break;
                            case key_class:
                                {
                                    //新的 class
                                    ++nProcess2;
                                    CInterfaceElement *p = CREATE_FROM_STATIC_POOL(CInterfaceElement);
                                    if (p)
                                    {
                                        p->m_pFather = this;
                                        int nRet = p->Process(ppElements,pTypes,nProcess2,nTotalEles);
                                        if (nRet >=0)
                                        {
                                            AddType(p);
                                            nProcess2 += nRet;
                                        }
                                        else
                                        {
                                            return nRet;
                                        }
                                    }
                                    else
                                    {
                                        char *pAt =NULL;
                                        if (nProcess2 < nTotalEles)
                                        {
                                            pAt = ppElements[nTotalEles];
                                        }
                                        OnError(pAt);
                                        return OUT_OF_MEM; 
                                    }
                                    ++nProcess2;
                                }
                                break;
                            case key_namespace   :
                                {
                                    //不该有
                                    char *pAt =NULL;
                                    if (nProcess2 < nTotalEles)
                                    {
                                        pAt = ppElements[nTotalEles];
                                    }
                                    OnError(pAt);
                                    return FAIL;
                                }
                                break;
                            case key_struct      :
                                {
                                    //暂时不处理   
                                    char *pAt =NULL;
                                    if (nProcess2 < nTotalEles)
                                    {
                                        pAt = ppElements[nTotalEles];
                                    }
                                    OnError(pAt);
                                    return FAIL;
                                }
                                break;
                            case key_public      :
                            case key_protected   :
                            case key_private     :
                                {
                                    ++nProcess2;
                                    if (operator_type == pTypes[nProcess2])
                                    {
                                        if (':' !=ppElements[nProcess2][0])
                                        {
                                            char *pAt =NULL;
                                            if (nProcess2 < nTotalEles)
                                            {
                                                pAt = ppElements[nTotalEles];
                                            }
                                            OnError(pAt);
                                            return -1;
                                        }
                                        else
                                        {
                                            ++nProcess2;
                                        }
                                    }
                                }
                                break;
                            case key_virtual:
                                {
                                    ++nProcess2;
                                    //后面肯定是一个函数
                                    if (HandleAStatement(ppElements,pTypes,nProcess2,nTotalEles) <= SUCCESS)
                                    {
                                        char *pAt =NULL;
                                        if (nProcess2 < nTotalEles)
                                        {
                                            pAt = ppElements[nTotalEles];
                                        }
                                        OnError(pAt);
                                        return -1;
                                    }
                                    else
                                    {
                                        CMethod *pMethod = dynamic_cast<CMethod *>(GetLastElement());
                                        if (pMethod)
                                        {
                                            //CParamerter *pPar = ( CParamerter *)pMethod->GetLastElement();
                                            pMethod->AddVirtualDeg();
                                        }
                                    }
                                }
                                break;
                            default:
                                {
                                    bNeedHandle = true;
                                }
                            }
                        }
                        if (bNeedHandle) //没有
                        {
                            //处理构造函数和析构函数
                            int nRet = HandleAStatement(ppElements,pTypes,nProcess2,nTotalEles);
                            if (nRet < SUCCESS)
                            {
                                return nRet;
                            }
                        }
                    }
                }
            } //case         en_class_left_brace:
            break;
        }
    }
    
    if (semicolon_type == pTypes[nProcess2])
    {
        return nProcess2 - nOld + 1;
    }
    return 0;
}

const char *CStruct::GetHierachyName()
{
    return m_szName.c_str();
}

TInt32 CStruct::GenerateSkeleton(const char *pPath)
{
    int nFileNr = 0;
    int nRet = GenerateSkeletonHeaderFile(pPath);
    if (nRet > SUCCESS)
    {
        ++nFileNr;
    }
    nRet = GenerateSkeletonSourceFile(pPath);
    if (nRet > SUCCESS)
    {
        ++nFileNr;
    }
    //一个skeleton\stub

    return nFileNr;
}

TInt32 CStruct::GenerateStub(const char *pPath)
{
    int nFileNr = 0;
    int nRet = GenerateStubHeaderFile(pPath);
    if (nRet > SUCCESS)
    {
        ++nFileNr;
    }
    nRet = GenerateStubSourceFile(pPath);
    if (nRet > SUCCESS)
    {
        ++nFileNr;
    }
    //一个skeleton\stub

    return nFileNr;
}


#define WRITE_LINE(PSZ_FORMAT,...)  n = sprintf(pBuff+nUsed,PSZ_FORMAT"\n",__VA_ARGS__); \
nUsed += n; \
nLength -=n; \

#define WRITE_CODE(PSZ_FORMAT,...)  n = sprintf(pBuff+nUsed,PSZ_FORMAT,__VA_ARGS__); \
    nUsed += n; \
    nLength -=n; \

TInt32 CStruct::GenerateMethodId(const char *pPath)
{
    {
        int nRet = 0;
        //stub 名字
        std::string szFileName = pPath;

        int nPathLen = szFileName.size();
        if (szFileName[nPathLen-1]=='/')
        {
            szFileName +="include/";
        }
        else
        {
            szFileName +="/include/";
        }
        szFileName += m_szName;
        szFileName += "GetLength.h";
        FILE *pFile = fopen(szFileName.c_str(),"w");
        int nLength = 2*1024*1024;
        int nUsed = 0;
        char *pBuff = NULL;
        NEW(pBuff,char,nLength);
        if (!pBuff)
        {
            return OUT_OF_MEM;
        }
        int n = 0;
        if (raw_namespace_type == m_pFather->m_nElmentType)
        {
            CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
            if (pNS)
            {
                n = pNS->GenerateUsingNamespaceCode(pBuff,nLength,0);
                nUsed += n;
                nLength -= n;
            }
        }
        WRITE_LINE("TInt32 GetLength(%s &rValue);",m_szName.c_str());
        WRITE_LINE("inline TInt32 GetLength(%s *pT);",m_szName.c_str());
        WRITE_LINE("{");
        WRITE_LINE("    return GetLength(*pT);");
        WRITE_LINE("}");


        fwrite(pBuff,1,nUsed,pFile);
        //sprintf_s()
        fclose (pFile);
        //delete [] pBuff;
    }

    {
        std::string szFileName = pPath;

        int nPathLen = szFileName.size();
        if (szFileName[nPathLen-1]=='/')
        {
            szFileName +="source/";
        }
        else
        {
            szFileName +="/source/";
        }
        szFileName += m_szName;
        szFileName += "GetLength.cpp";
        FILE *pFile = fopen(szFileName.c_str(),"w");
        int nLength = 2*1024*1024;
        int nUsed = 0;
        char *pBuff = NULL;
        NEW(pBuff,char,nLength);
        if (!pBuff)
        {
            return OUT_OF_MEM;
        }
        int n = 0;
        if (raw_namespace_type == m_pFather->m_nElmentType)
        {
            CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
            if (pNS)
            {
                n = pNS->GenerateUsingNamespaceCode(pBuff,nLength,0);
                nUsed += n;
                nLength -= n;
            }
        }
        WRITE_LINE("TInt32 GetLength(%s &rValue)",m_szName.c_str());
        WRITE_LINE("{");
        if (0 == m_tChilds.size())
        {
            printf("Error ,find empty struct!");
        }
        else
        {
            n = sprintf(pBuff+nUsed,"return ");
            nUsed += n;
            nLength -= n;
            //int nPar = 0;
            for (int i=0;i<m_tChilds.size();++i)
            {
                CBaseElement *pBase = m_tChilds[i].m_pPt;
                if (raw_parameter_type == pBase->m_nElmentType)
                {
                    CParamerter *pParm = dynamic_cast<CParamerter*>(pBase);
                    if (pParm)
                    {
                        if (0 == i)
                        {
                            n = sprintf(pBuff+nUsed, "GetLength(rValue.%s",pParm->m_szName.c_str());
                            nLength -= n;
                            nUsed += n;
                            if (pParm->m_pFullType->GetDimension() > 0)
                            {
                                for (int i=0;i<pParm->m_pFullType->GetDimension();++i)
                                {
                                    n = sprintf(pBuff+nUsed,"[0]");
                                    nLength -= n;
                                    nUsed += n;
                                }
                                n = sprintf(pBuff+nUsed,")");
                                nLength -= n;
                                nUsed += n;
                                for (int i=0;i<pParm->m_pFullType->GetDimension();++i)
                                {
                                    n = sprintf(pBuff+nUsed,"*(%s)",pParm->m_pFullType->GetDimension(i)->c_str());
                                    nLength -= n;
                                    nUsed += n;
                                }
                            }
                            else
                            {
                                n = sprintf(pBuff+nUsed,")");
                                nLength -= n;
                                nUsed += n;
                            }
                        }
                        else
                        {
                            n = sprintf(pBuff+nUsed, "+GetLength(rValue.%s",pParm->m_szName.c_str());
                            nLength -= n;
                            nUsed += n;
                            if (pParm->m_pFullType->GetDimension() > 0)
                            {
                                for (int i=0;i<pParm->m_pFullType->GetDimension();++i)
                                {
                                    n = sprintf(pBuff+nUsed,"[0]");
                                    nLength -= n;
                                    nUsed += n;
                                }
                                n = sprintf(pBuff+nUsed,")");
                                nLength -= n;
                                nUsed += n;
                                for (int i=0;i<pParm->m_pFullType->GetDimension();++i)
                                {
                                    n = sprintf(pBuff+nUsed,"*(%s)",pParm->m_pFullType->GetDimension(i)->c_str());
                                    nLength -= n;
                                    nUsed += n;
                                }
                            }
                            else
                            {
                                n = sprintf(pBuff+nUsed,")");
                                nLength -= n;
                                nUsed += n;
                            }
                        }
                    }
                }
            }
            
        }

        WRITE_LINE(";\n}");


        fwrite(pBuff,1,nUsed,pFile);
        //sprintf_s()
        fclose (pFile);
        //delete [] pBuff;
        return nUsed;
    }
}

TInt32 CStruct::GenerateStubHeaderFile(const char *pPath) //生成Marshaller.h
{
    int nUsed = 0;
    //stub 名字
    std::string szFileName = pPath;
    int nPathLen = szFileName.size();
    if (szFileName[nPathLen-1]=='/')
    {
        szFileName +="include/";
    }
    else
    {
        szFileName +="/include/";
    }
    szFileName += m_szName;
    szFileName += "Marshaller.h";
    FILE *pFile = fopen(szFileName.c_str(),"w");
    int nLength = 2*1024*1024;
    char *pBuff = NULL;
    NEW(pBuff,char,nLength);
    if (!pBuff)
    {
        return OUT_OF_MEM;
    }
    int n = 0;
    WRITE_LINE("#include \"Public/include/Typedef.h\"");
    WRITE_LINE("#include \"%sGetLength.h\"",m_szName.c_str());
    WRITE_LINE("#include \"%s\"",CHeaderFile::m_pFileName,m_szName.c_str());
    if (raw_namespace_type == m_pFather->m_nElmentType)
    {
        CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
        if (pNS)
        {
            int n = pNS->GenerateUsingNamespaceCode(pBuff,nLength,0);
            nUsed += n;
            nLength -= n;
        }
    }
    WRITE_LINE("TInt32 Marshall(TUChar *pBuff,TInt32 nLength,%s &_rValue);",m_szName.c_str());
    WRITE_LINE("inline TInt32 Marshall(TUChar *pBuff,TInt32 nLength,%s *pT);",m_szName.c_str());
    WRITE_LINE("{");
    WRITE_LINE("    return Marshall(pBuff,nLength,*pT);");
    WRITE_LINE("}");

    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);
    delete [] pBuff;
    return nUsed;
}

TInt32 CStruct::GenerateStubSourceFile(const char *pPath) //生成Marshaller.cpp
{
    std::string szFileName = pPath;
    int nPathLen = szFileName.size();
    if (szFileName[nPathLen-1]=='/')
    {
        szFileName +="source/";
    }
    else
    {
        szFileName +="/source/";
    }
    szFileName += m_szName;
    szFileName += "Marshaller.cpp";
    FILE *pFile = fopen(szFileName.c_str(),"w");
    int nLength = 2*1024*1024;
    char *pBuff = NULL;
    int n=0;
    TInt32 nUsed = 0;
    NEW(pBuff,char,nLength);
    if (!pBuff)
    {
        return OUT_OF_MEM;
    }
    WRITE_LINE("#include \"Public/include/TypeMarshaller.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../include/%sMarshaller.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../include/%sGetLength.h\"",m_szName.c_str());
    WRITE_LINE("TInt32 Marshall(TUChar *pBuff,TInt32 nLength,%s &_rValue)",m_szName.c_str());
    WRITE_LINE("{");
    WRITE_LINE("    TInt32 nUsed=0;");
    WRITE_LINE("    TInt32 n = 0;");
    
    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *pBase = m_tChilds[i].m_pPt;
        if (raw_parameter_type == pBase->m_nElmentType)
        {
            CParamerter *pParm = dynamic_cast<CParamerter*>(pBase);
            if (pParm)
            {
                int nDimension = pParm->m_pFullType->GetDimension();
                if (nDimension>0)
                {
                    for (int j=0;j<nDimension;++j)
                    {
                        n = WriteEtch(pBuff+nUsed,j+1);
                        nUsed += n;
                        nLength -= n;
                        char c = 'i'+j;
                        WRITE_CODE("for(int %c=0;%c<%s;++%c)\n",c,c,pParm->m_pFullType->GetDimension(j)->c_str(),c);
                        n = WriteEtch(pBuff+nUsed,j+1);
                        nUsed += n;
                        nLength -= n;
                        WRITE_CODE("{\n");
                    }
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_CODE("    n = Marshall(pBuff+nUsed,nLength,rValue.%s",pParm->m_szName.c_str());
                    for (int j=0;j<nDimension;++j)
                    {
                        char c = 'i'+j;
                        WRITE_CODE("[%c]",c);
                    }
                    WRITE_CODE(");\n");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    if (n < SUCCESS)");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    {");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("      return n;");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    }");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    nUsed += n;");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    nLength-=n;");
                    for (int j=nDimension;j>0;--j)
                    {
                        n = WriteEtch(pBuff+nUsed,j);
                        nUsed += n;
                        nLength -= n;
                        WRITE_CODE("}\n");
                    }
                }
                else
                {
                    WRITE_LINE("    n = Marshall(pBuff+nUsed,nLength,rValue.%s);\n",pParm->m_szName.c_str());
                    WRITE_LINE("    if (n < SUCCESS)");
                    WRITE_LINE("    {");
                    WRITE_LINE("      return n;");
                    WRITE_LINE("    }");
                    WRITE_LINE("    nUsed += n;");
                    WRITE_LINE("    nLength-=n;");
                }
            }
        }
    }
    WRITE_LINE("    return nUsed;");
    WRITE_LINE("}");
    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);
    delete [] pBuff;

    return SUCCESS;
}

TInt32 CStruct::GenerateSkeletonHeaderFile(const char *pPath) //生成UnMarshaller.h
{
    std::string szFileName = pPath;
    int nPathLen = szFileName.size();
    if (szFileName[nPathLen-1]=='/')
    {
        szFileName +="include/";
    }
    else
    {
        szFileName +="/include/";
    }
    szFileName += m_szName;
    szFileName += "Unmarshaller.h";
    FILE *pFile = fopen(szFileName.c_str(),"w");
    int nUsed = 0;
    int nLength = 2*1024*1024;
    char *pBuff = NULL;
    NEW(pBuff,char,nLength);
    if (!pBuff)
    {
        return OUT_OF_MEM;
    }
    int n = 0;
    WRITE_LINE("#include \"Public/include/Typedef.h\"");
    WRITE_LINE("#include \"%sGetLength.h\"",m_szName.c_str());
    WRITE_LINE("#include \"%s\"",CHeaderFile::m_pFileName,m_szName.c_str());
    if (raw_namespace_type == m_pFather->m_nElmentType)
    {
        CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
        if (pNS)
        {
            int n = pNS->GenerateUsingNamespaceCode(pBuff,nLength,0);
            nUsed += n;
            nLength -= n;
        }
    }
    WRITE_LINE("TInt32 Unmarshall(TUChar *pBuff,TInt32 nLength,%s &_rValue);",m_szName.c_str());
    WRITE_LINE("inline TInt32 Unmarshall(TUChar *pBuff,TInt32 nLength,%s *&pT);",m_szName.c_str());
    WRITE_LINE("{");
    WRITE_LINE("    return Unmarshall(*pT);");
    WRITE_LINE("}");

    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);
    delete [] pBuff;
    return nUsed;
}

TInt32 CStruct::GenerateSkeletonSourceFile(const char *pPath) //生成UnMarshaller.cpp
{
    std::string szFileName = pPath;
    int nPathLen = szFileName.size();
    if (szFileName[nPathLen-1]=='/')
    {
        szFileName +="source/";
    }
    else
    {
        szFileName +="/source/";
    }
    szFileName += m_szName;
    szFileName += "Unmarshaller.cpp";
    FILE *pFile = fopen(szFileName.c_str(),"w");
    int nLength = 2*1024*1024;
    char *pBuff = NULL;
    NEW(pBuff,char,nLength);
    if (!pBuff)
    {
        return OUT_OF_MEM;
    }
    int nUsed = 0;
    int n = 0;
    WRITE_LINE("#include \"Public/include/TypeMarshaller.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../include/%sUnmarshaller.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../include/%sGetLength.h\"",m_szName.c_str());
    WRITE_LINE("TInt32 UnMarshall(TUChar *pBuff,TInt32 nLength,%s &_rValue)",m_szName.c_str());
    WRITE_LINE("{");
    WRITE_LINE("    TInt32 nUsed=0;");
    WRITE_LINE("    TInt32 n = 0;");



    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *pBase = m_tChilds[i].m_pPt;
        if (raw_parameter_type == pBase->m_nElmentType)
        {
            CParamerter *pParm = dynamic_cast<CParamerter*>(pBase);
            if (pParm)
            {
                int nDimension = pParm->m_pFullType->GetDimension();
                if (nDimension > 0)
                {
                    for (int j=0;j<nDimension;++j)
                    {
                        n = WriteEtch(pBuff+nUsed,j+1);
                        nUsed += n;
                        nLength -= n;
                        char c = 'i'+j;
                        WRITE_CODE("for(int %c=0;%c<%s;++%c)\n",c,c,pParm->m_pFullType->GetDimension(j)->c_str(),c);
                        n = WriteEtch(pBuff+nUsed,j+1);
                        nUsed += n;
                        nLength -= n;
                        WRITE_CODE("{\n");
                    }
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_CODE("    n = Unmarshall(pBuff+nUsed,nLength,rValue.%s",pParm->m_szName.c_str());
                    for (int j=0;j<nDimension;++j)
                    {
                        char c = 'i'+j;
                        WRITE_CODE("[%c]",c);
                    }
                    WRITE_CODE(");\n");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    if (n < SUCCESS)");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    {");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("      return n;");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    }");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    nUsed += n;");
                    n = WriteEtch(pBuff+nUsed,nDimension);
                    nUsed += n;
                    nLength -= n;
                    WRITE_LINE("    nLength-=n;");
                    for (int j=nDimension;j>0;--j)
                    {
                        n = WriteEtch(pBuff+nUsed,j);
                        nUsed += n;
                        nLength -= n;
                        WRITE_CODE("}\n");
                    }
                }
                else
                {
                    WRITE_LINE("    n = Unmarshall(pBuff+nUsed,nLength,_rValue.%s);",pParm->m_szName.c_str());
                    WRITE_LINE("    if (n < SUCCESS)");
                    WRITE_LINE("    {");
                    WRITE_LINE("      return n;");
                    WRITE_LINE("    }");
                    WRITE_LINE("    nUsed += n;");
                    WRITE_LINE("    nLength-=n;");
                }
            }
        }
    }
    WRITE_LINE("    return nUsed;");
    WRITE_LINE("}");

    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);
    delete [] pBuff;
    return SUCCESS;
}

int CStruct::HandleAStatement(char **ppElements,EnType *pTypes,int &nProcess2,int nTotalEles)
{
    CFullTypeDef *pFullType = CREATE_FROM_STATIC_POOL(CFullTypeDef);
    if (!pFullType)
    {
        char *pAt =NULL;
        if (nProcess2 < nTotalEles)
        {
            pAt = ppElements[nTotalEles];
        }
        OnError(pAt);
        return OUT_OF_MEM;
    }
    int nRet = pFullType->Process(ppElements,pTypes,nProcess2,nTotalEles);
    if (nRet > SUCCESS) //必须有
    {
        nProcess2 += nRet;
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
    //find a name
    if (nProcess2 >= nTotalEles)
    {
        char *pAt =NULL;
        if (nProcess2 < nTotalEles)
        {
            pAt = ppElements[nTotalEles];
        }
        OnError(pAt);
        return INCORRECT_END;
    }
    if(alphabet_type == pTypes[nProcess2])
    {
        //a name;
        char *pName = ppElements[nProcess2];
        ++nProcess2;
        if (nProcess2 >= nTotalEles)
        {
            char *pAt =NULL;
            if (nProcess2 < nTotalEles)
            {
                pAt = ppElements[nTotalEles];
            }
            OnError(pAt);
            return INCORRECT_END;
        }
        
        {
            //处理数组？ 
            while(semicolon_type != pTypes[nProcess2])
            {
                if (operator_type == pTypes[nProcess2])
                {
                    if ('[' == ppElements[nProcess2][0])
                    {
                        ++nProcess2;
                        pFullType->AddDimension(ppElements[nProcess2]);
                        ++nProcess2;
                        if (']' != ppElements[nProcess2][0])
                        {
                            OnError(ppElements[nProcess2]);
                            return INCORRECT_END;
                        }
                    }
                }
                ++nProcess2;
                if (nProcess2 >= nTotalEles)
                {
                    return INCORRECT_END;
                }
            }
            CParamerter *pPar = CREATE_FROM_STATIC_POOL(CParamerter);
            if (!pPar)
            {
                OnError(ppElements[nProcess2]);
                return OUT_OF_MEM;
            }
            ++nProcess2; //处理一个
            pPar->SetType(pFullType);
            pPar->SetName(pName);
            AddChildElement(pPar);
            //忽略成員.
            return 0;
        }
    }
    else
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


}
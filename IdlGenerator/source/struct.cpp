#include "../include/Struct.h"
#include "../include/HeaderFile.h"
#include "../include/interfaceElement.h"
#include "../include/FullTypeClass.h"
#include "../include/IdlGeneratorErrorCode.h"
#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/nameSpace.h"
#pragma warning(push)
#pragma warning(disable:4267)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4100)
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


TInt32 CStruct::GenerateMethodId(const char *pPath)
{
    {
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
        char szMacro[128];
        n = sprintf(szMacro,"__%s_GET_LENGTH_H__",m_szName.c_str());

        for (int i=0;i<n;++i)
        {
            szMacro[i] = toupper(szMacro[i]);
        }

        n = sprintf(pBuff,"#ifndef %s\n#define %s\n",(szMacro),szMacro);
        nUsed += n;
        nLength -= n;

        WRITE_LINE("#include \"Public/include/Typedef.h\"");
        WRITE_LINE("#include \"../Interface/%s\"",CHeaderFile::m_pFileName);
        if (raw_namespace_type == m_pFather->m_nElmentType)
        {
            CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
            if (pNS)
            {
                n = pNS->GenerateUsingNamespaceCode(pBuff+nUsed,nLength,0);
                nUsed += n;
                nLength -= n;
            }
        }
        WRITE_LINE("TInt32 GetLength(const %s &_rValue);",m_szName.c_str());
        WRITE_LINE("TInt32 GetLength(const %s *_pT);",m_szName.c_str());
        WRITE_LINE("#endif");

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
     
        WRITE_LINE("#include \"../include/%sGetLength.h\"",m_szName.c_str());
        WRITE_LINE("#include \"Public/include/TypeMarshaller.h\"");
        if (raw_namespace_type == m_pFather->m_nElmentType)
        {
            CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
            if (pNS)
            {
                n = pNS->GenerateUsingNamespaceCode(pBuff+nUsed,nLength,0);
                nUsed += n;
                nLength -= n;
            }
        }
        WRITE_LINE("TInt32 GetLength(const %s *_pT)",m_szName.c_str());
        WRITE_LINE("{");
        WRITE_LINE("    return GetLength(*_pT);");
        WRITE_LINE("}");
        WRITE_LINE("TInt32 GetLength(const %s &_rValue)",m_szName.c_str());
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
                            n = sprintf(pBuff+nUsed, "GetLength(_rValue.%s",pParm->m_szName.c_str());
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
                            n = sprintf(pBuff+nUsed, "+GetLength(_rValue.%s",pParm->m_szName.c_str());
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

    char szMacro[128];
    n = sprintf(szMacro,"__%s_MARSHALLER_H__",m_szName.c_str());

    for (int i=0;i<n;++i)
    {
        szMacro[i] = toupper(szMacro[i]);
    }

    n = sprintf(pBuff,"#ifndef %s\n#define %s\n",(szMacro),szMacro);
    nUsed += n;
    nLength -= n;
    WRITE_LINE("#include \"Public/include/Typedef.h\"");
    WRITE_LINE("#include \"%sGetLength.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../Interface/%s\"",CHeaderFile::m_pFileName);
    if (raw_namespace_type == m_pFather->m_nElmentType)
    {
        CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
        if (pNS)
        {
            int n = pNS->GenerateUsingNamespaceCode(pBuff+nUsed,nLength,0);
            nUsed += n;
            nLength -= n;
        }
    }
    WRITE_LINE("TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const %s &_rValue);",m_szName.c_str());
    WRITE_LINE("TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const %s *_pT);",m_szName.c_str());
    WRITE_LINE("#endif");
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
    if (m_pFather)
    {
        for (int i=0;i<m_pFather->m_tChilds.size();++i)
        {
            CBaseElement *pBase = m_pFather->m_tChilds[i].m_pPt;
            if ((raw_struct_type == pBase->m_nElmentType)&&(pBase != this))
            {
                n = sprintf(pBuff+nUsed,"#include \"../include/%sMarshaller.h\"\n",pBase->m_szName.c_str());
                nUsed += n;
                nLength -= n;
                n = sprintf(pBuff+nUsed,"#include \"../include/%sGetLength.h\"\n",pBase->m_szName.c_str());
                nUsed += n;
                nLength -= n;
            }
        }
    }
    WRITE_LINE("TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const %s *_pT)",m_szName.c_str());
    WRITE_LINE("{");
    WRITE_LINE("    return Marshall(_pBuff,_nLength,*_pT);");
    WRITE_LINE("}");
    WRITE_LINE("TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const %s &_rValue)",m_szName.c_str());
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
                    WRITE_CODE("    n = Marshall(_pBuff+nUsed,_nLength,_rValue.%s",pParm->m_szName.c_str());
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
                    WRITE_LINE("    _nLength-=n;");
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
                    WRITE_LINE("    n = Marshall(_pBuff+nUsed,_nLength,_rValue.%s);\n",pParm->m_szName.c_str());
                    WRITE_LINE("    if (n < SUCCESS)");
                    WRITE_LINE("    {");
                    WRITE_LINE("      return n;");
                    WRITE_LINE("    }");
                    WRITE_LINE("    nUsed += n;");
                    WRITE_LINE("    _nLength-=n;");
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
    char szMacro[128];
    n = sprintf(szMacro,"__%s_UNMARSHALLER_H__",m_szName.c_str());

    for (int i=0;i<n;++i)
    {
        szMacro[i] = toupper(szMacro[i]);
    }

    n = sprintf(pBuff,"#ifndef %s\n#define %s\n",(szMacro),szMacro);
    nUsed += n;
    nLength -= n;
    WRITE_LINE("#include \"Public/include/Typedef.h\"");
    WRITE_LINE("#include \"%sGetLength.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../Interface/%s\"",CHeaderFile::m_pFileName,m_szName.c_str());
    if (raw_namespace_type == m_pFather->m_nElmentType)
    {
        CNamespace *pNS = dynamic_cast<CNamespace *>(m_pFather);
        if (pNS)
        {
            int n = pNS->GenerateUsingNamespaceCode(pBuff+nUsed,nLength,0);
            nUsed += n;
            nLength -= n;
        }
    }
    WRITE_LINE("TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,%s &_rValue);",m_szName.c_str());
    WRITE_LINE("TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,%s *&_pT);",m_szName.c_str());
    WRITE_LINE("#endif");
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
    WRITE_LINE("#include \"Public/include/TypeUnmarshaller.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../include/%sUnmarshaller.h\"",m_szName.c_str());
    WRITE_LINE("#include \"../include/%sGetLength.h\"",m_szName.c_str());
    if (m_pFather)
    {
        for (int i=0;i<m_pFather->m_tChilds.size();++i)
        {
            CBaseElement *pBase = m_pFather->m_tChilds[i].m_pPt;
            if ((raw_struct_type == pBase->m_nElmentType)&&(pBase != this))
            {
                n = sprintf(pBuff+nUsed,"#include \"../include/%sUnmarshaller.h\"\n",pBase->m_szName.c_str());
                nUsed += n;
                nLength -= n;
            }
        }
    }

    WRITE_LINE("inline TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,%s *&_pT)",m_szName.c_str());
    WRITE_LINE("{");
    WRITE_LINE("    return Unmarshall(_pBuff,_nLength,*_pT);");
    WRITE_LINE("}");

    WRITE_LINE("TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,%s &_rValue)",m_szName.c_str());
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
                    WRITE_CODE("    n = Unmarshall(_pBuff+nUsed,_nLength,_rValue.%s",pParm->m_szName.c_str());
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
                    WRITE_LINE("    _nLength-=n;");
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
                    WRITE_LINE("    n = Unmarshall(_pBuff+nUsed,_nLength,_rValue.%s);",pParm->m_szName.c_str());
                    WRITE_LINE("    if (n < SUCCESS)");
                    WRITE_LINE("    {");
                    WRITE_LINE("      return n;");
                    WRITE_LINE("    }");
                    WRITE_LINE("    nUsed += n;");
                    WRITE_LINE("    _nLength-=n;");
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

TInt32 CStruct::GenerateCSharpCode(const char *pPath,int nChoice)
{
    int nRet=0;
    if (nChoice & 0x01)
    {
        nRet= GenerateCSharpSkeleton(pPath);
        if (nRet < SUCCESS)
        {
            return nRet;
        }
    }
    if (nChoice & 0x02)
    {
        nRet = GenerateCSharpStub(pPath);
    }
    return  nRet;
}


TInt32 CStruct::GenerateCSharpSkeleton(const char*pPath)
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
    szFileName +=m_szName;
    szFileName += "Marshaller.cs";
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
    WRITE_LINE("using System.Collections;");
    WRITE_LINE("using System.Collections.Generic;");
    WRITE_LINE("using System;");

    WRITE_LINE("public class %s",m_szName.c_str());
    WRITE_LINE("{");
    int nEtchNr=1;
    for(int i=0;i<m_tChilds.size();++i)
    {
        //call
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = (CParamerter*)p;
            const string *pCsType =  GetCSharpType(pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str());
            const TChar *pszType;
            if (pCsType)
            {
                pszType = pCsType->c_str();
            }
            else
            {
                pszType = pPar->m_pFullType->GetCSharpTypeCode()->c_str();
            }
            if (pPar->m_pFullType->GetDimension())
            {
                WRITE_CODE_ETCH("%s[",pszType);
                for (int i=1;i<pPar->m_pFullType->GetDimension();++i)
                {
                    WRITE_CODE(",");
                }
                const TChar *pszNr = pPar->m_pFullType->GetDimension(0)->c_str();
                if (('0'<pszNr[0])&&(pszNr[0]<'9'))
                {
                    WRITE_CODE("] %s = new %s[%s",pPar->m_szName.c_str(),pszType,pszNr);
                }
                else
                {
                    WRITE_CODE("] %s = new %s[MacrosAndDef.%s",pPar->m_szName.c_str(),pszType,pszNr);
                }
                
                for (int j=1;j<pPar->m_pFullType->GetDimension();++j)
                {
                    pszNr = pPar->m_pFullType->GetDimension(j)->c_str();
                    if (('0'<pszNr[0])&&(pszNr[0]<'9'))
                    {
                        WRITE_CODE(",%s",pszNr);
                    }
                    else
                    {
                        WRITE_CODE(",MacrosAndDef.%s",pszNr);
                    }
                }
                WRITE_LINE("];");
            }
            else
            {
                WRITE_LINE_ETCH("%s %s;",pszType,pPar->m_szName.c_str());
            }
        }
        //return
    }
    WRITE_LINE_ETCH("public %s()",m_szName.c_str());
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    for(int i=0;i<m_tChilds.size();++i)
    {
        //call
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = (CParamerter*)p;
            
            if (pPar->m_pFullType->GetDimension())
            {
                const string *pCsType =  GetCSharpType(pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str());
                const TChar *pszType;
                if (pCsType)
                {
                    pszType = pCsType->c_str();
                }
                else
                {
                    pszType = pPar->m_pFullType->GetCSharpTypeCode()->c_str();
                }
                int nDimension = pPar->m_pFullType->GetDimension();
                for (int j=0;j<nDimension;++j)
                {
                    char c = 'i'+j;
                    
                    const TChar *pszNr = pPar->m_pFullType->GetDimension(j)->c_str();
                    if (('0'<=pszNr[0])&&(pszNr[0]<='9'))
                    {
                        WRITE_LINE_ETCH("for(int %c=0;%c<%s;++%c)",c,c,pPar->m_pFullType->GetDimension(j)->c_str(),c);
                    }
                    else
                    {
                        WRITE_LINE_ETCH("for(int %c=0;%c<MacrosAndDef.%s;++%c)",c,c,pPar->m_pFullType->GetDimension(j)->c_str(),c);
                    }

                    WRITE_LINE_ETCH("{");
                    ++nEtchNr;
                }
                WRITE_CODE_ETCH("%s[",pPar->m_szName.c_str());

                for (int j=0;j<nDimension;++j)
                {
                    char c = 'i'+j;
                    if (0==j)
                    {
                        WRITE_CODE("%c",c);
                    }
                    else
                    {
                        WRITE_CODE(",%c",c);
                    }
                }
                {
                    WRITE_LINE("] = new %s();",pszType);
                }
               
                for (int j=nDimension;j>0;--j)
                {
                    --nEtchNr;
                    WRITE_LINE_ETCH("}");
                }
            }
        }
    }
    --nEtchNr;
    WRITE_LINE_ETCH("}");

    WRITE_LINE_ETCH("public static int Unmarshall(byte[] pBuffers,int nBufferLen,int nUsed, out %s rValue)",m_szName.c_str());
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("rValue = new %s();",m_szName.c_str());
    WRITE_LINE_ETCH("int nLen=0;");
    for(int i=0;i<m_tChilds.size();++i)
    {
        //call
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = (CParamerter*)p;
            const string *pCsType =  GetCSharpType(pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str());
            if (pPar->m_pFullType->GetDimension())
            {
                int nDimension = pPar->m_pFullType->GetDimension();
                for (int j=0;j<nDimension;++j)
                {
                    char c = 'i'+j;
                    const TChar *pszNr = pPar->m_pFullType->GetDimension(j)->c_str();
                    if (('0'<=pszNr[0])&&(pszNr[0]<='9'))
                    {
                        WRITE_LINE_ETCH("for(int %c=0;%c<%s;++%c)",c,c,pPar->m_pFullType->GetDimension(j)->c_str(),c);
                    }
                    else
                    {
                        WRITE_LINE_ETCH("for(int %c=0;%c<MacrosAndDef.%s;++%c)",c,c,pPar->m_pFullType->GetDimension(j)->c_str(),c);
                    }
                    WRITE_LINE_ETCH("{");
                    ++nEtchNr;
                }

                if (pCsType)
                {
                    WRITE_CODE_ETCH("nLen = TypeUnmarshaller.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.%s[",pPar->m_szName.c_str());
                }
                else
                {
                    WRITE_CODE_ETCH("nLen = %s.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.%s[",pPar->m_pFullType->GetCSharpTypeCode()->c_str(),pPar->m_szName.c_str());
                }

                for (int j=0;j<nDimension;++j)
                {
                    char c = 'i'+j;
                    if (0==j)
                    {
                        WRITE_CODE("%c",c);
                    }
                    else
                    {
                        WRITE_CODE(",%c",c);
                    }
                }
                
                WRITE_LINE("]);")

                WRITE_LINE_ETCH("if (nLen < MacrosAndDef.SUCCESS)");
                WRITE_LINE_ETCH("{");
                ++nEtchNr;
                WRITE_LINE_ETCH("return MacrosAndDef.OUT_OF_MEM;");
                --nEtchNr;
                WRITE_LINE_ETCH("}");
                WRITE_LINE_ETCH("nUsed += nLen;");

                for (int j=nDimension;j>0;--j)
                {
                    --nEtchNr;
                    WRITE_LINE_ETCH("}");
                }
            }
            else
            {
                if (pCsType)
                {
                    WRITE_LINE_ETCH("nLen = TypeUnmarshaller.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.%s);",pPar->m_szName.c_str());
                }
                else
                {
                    WRITE_LINE_ETCH("nLen = %s.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.%s);",pPar->m_pFullType->GetCSharpTypeCode()->c_str(),pPar->m_szName.c_str());
                }
                WRITE_LINE_ETCH("if (nLen < MacrosAndDef.SUCCESS)");
                WRITE_LINE_ETCH("{");
                ++nEtchNr;
                WRITE_LINE_ETCH("return MacrosAndDef.OUT_OF_MEM;");
                --nEtchNr;
                WRITE_LINE_ETCH("}");
                WRITE_LINE_ETCH("nUsed += nLen;");
            }
        }
        //return
    }
    WRITE_LINE_ETCH("return nUsed;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");

    WRITE_LINE_ETCH("public static int GetLength(%s rValue)",m_szName.c_str());
    WRITE_LINE_ETCH("{");
    ++nEtchNr;

    WRITE_CODE_ETCH("return ");
    for(int i=0;i<m_tChilds.size();++i)
    {
        //call
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (i)
        {
            WRITE_CODE("+");
        }
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = (CParamerter*)p;
            const string *pCsType =  GetCSharpType(pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str());
            if (pPar->m_pFullType->GetDimension())
            {
                if (pCsType)
                {
                    WRITE_CODE("TypeMarshaller.GetLength(rValue.%s[0",pPar->m_szName.c_str());
                }
                else
                {
                    WRITE_CODE("%s.GetLength(rValue.%s[0",pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str(),pPar->m_szName.c_str());
                }
                for (int j=1;j<pPar->m_pFullType->GetDimension();++j)
                {
                    WRITE_CODE(",0");
                }

                const TChar *pszNr = pPar->m_pFullType->GetDimension(0)->c_str();
                if (('0'<=pszNr[0])&&(pszNr[0]<='9'))
                {
                    WRITE_CODE("])*%s",pszNr);
                }
                else
                {
                    WRITE_CODE("])*MacrosAndDef.%s",pszNr);
                }

                for (int j=1;j<pPar->m_pFullType->GetDimension();++j)
                {
                    pszNr = pPar->m_pFullType->GetDimension(j)->c_str();
                    if (('0'<pszNr[0])&&(pszNr[0]<'9'))
                    {
                        WRITE_CODE("*%s",pszNr);
                    }
                    else
                    {
                        WRITE_CODE("*MacrosAndDef.%s",pszNr);
                    }
                }
            }
            else
            {
                if (pCsType)
                {
                    WRITE_CODE("TypeMarshaller.GetLength(rValue.%s)",pPar->m_szName.c_str());
                }
                else
                {
                    WRITE_CODE("%s.GetLength(rValue.%s)",pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str(),pPar->m_szName.c_str());
                }
            }
        }
    }
    WRITE_LINE(";");
    --nEtchNr;
    WRITE_LINE_ETCH("}");

    WRITE_LINE_ETCH("public static int Marshall(byte[] pBuffers,int nBufferLen,int nUsed, %s rValue)",m_szName.c_str());
    WRITE_LINE_ETCH("{");
    ++nEtchNr;

    WRITE_LINE_ETCH("int nLen=0;");
    WRITE_LINE_ETCH("int n;");
    for(int i=0;i<m_tChilds.size();++i)
    {
        //call
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = (CParamerter*)p;
            const string *pCsType =  GetCSharpType(pPar->m_pFullType->GetCSharpBaseTypeCode()->c_str());
            if (pPar->m_pFullType->GetDimension())
            {
                int nDimension = pPar->m_pFullType->GetDimension();
                for (int j=0;j<nDimension;++j)
                {
                    char c = 'i'+j;
                    const TChar *pszNr = pPar->m_pFullType->GetDimension(j)->c_str();
                    if (('0'<=pszNr[0])&&(pszNr[0]<='9'))
                    {
                        WRITE_LINE_ETCH("for(int %c=0;%c<%s;++%c)",c,c,pPar->m_pFullType->GetDimension(j)->c_str(),c);
                    }
                    else
                    {
                        WRITE_LINE_ETCH("for(int %c=0;%c<MacrosAndDef.%s;++%c)",c,c,pPar->m_pFullType->GetDimension(j)->c_str(),c);
                    }
                    WRITE_LINE_ETCH("{");
                    ++nEtchNr;
                }

                if (pCsType)
                {
                    WRITE_CODE_ETCH("n = TypeMarshaller.Marshall(pBuffers, nBufferLen, nUsed, rValue.%s[",pPar->m_szName.c_str());
                }
                else
                {
                    WRITE_CODE_ETCH("n = %s.Marshall(pBuffers, nBufferLen, nUsed, rValue.%s[",pPar->m_pFullType->GetCSharpTypeCode()->c_str(),pPar->m_szName.c_str());
                }

                for (int j=0;j<nDimension;++j)
                {
                    char c = 'i'+j;
                    if (0==j)
                    {
                        WRITE_CODE("%c",c);
                    }
                    else
                    {
                        WRITE_CODE(",%c",c);
                    }
                }

                WRITE_LINE("]);")

                WRITE_LINE_ETCH("if (n < MacrosAndDef.SUCCESS)");
                WRITE_LINE_ETCH("{");
                ++nEtchNr;
                WRITE_LINE_ETCH("return MacrosAndDef.OUT_OF_MEM;");
                --nEtchNr;
                WRITE_LINE_ETCH("}");
                WRITE_LINE_ETCH("nLen += n;");
                WRITE_LINE_ETCH("nUsed += n;");

                for (int j=nDimension;j>0;--j)
                {
                    --nEtchNr;
                    WRITE_LINE_ETCH("}");
                }
            }
            else
            {
                if (pCsType)
                {
                    WRITE_LINE_ETCH("n = TypeMarshaller.Marshall(pBuffers, nBufferLen, nUsed, rValue.%s);",pPar->m_szName.c_str());
                }
                else
                {
                    WRITE_LINE_ETCH("n = %s.Marshall(pBuffers, nBufferLen, nUsed, rValue.%s);",pPar->m_pFullType->GetCSharpTypeCode()->c_str(),pPar->m_szName.c_str());
                }
                WRITE_LINE_ETCH("if (n < MacrosAndDef.SUCCESS)");
                WRITE_LINE_ETCH("{");
                ++nEtchNr;
                WRITE_LINE_ETCH("return MacrosAndDef.OUT_OF_MEM;");
                --nEtchNr;
                WRITE_LINE_ETCH("}");
                WRITE_LINE_ETCH("nLen += n;");
                WRITE_LINE_ETCH("nUsed += n;");
            }
        }
        //return
    }
    WRITE_LINE_ETCH("return nUsed;");
    --nEtchNr;
    WRITE_LINE_ETCH("}");



    
    n = GenerateCommonTypeMarshallerCSharpCode(pBuff+nUsed,m_szName.c_str());
    nUsed += n;
    nLength -= n;

    n = GenerateCommonTypeUnMarshallerCSharpCode(pBuff+nUsed,m_szName.c_str());
    nUsed += n;
    nLength -= n;


    --nEtchNr;
    WRITE_LINE_ETCH("}");

    nUsed = Replace4CSharp(pBuff);
    fwrite(pBuff,1,nUsed,pFile);
    //sprintf_s()
    fclose (pFile);
    delete [] pBuff;
    pBuff = NULL;
    return SUCCESS;
}

TInt32 CStruct::GenerateCSharpStub(const char*pPath)
{
    return SUCCESS;
}


}

#pragma warning(pop)
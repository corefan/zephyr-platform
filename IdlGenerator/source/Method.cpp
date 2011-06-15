#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/IdlGeneratorErrorCode.h"
#include "../include/interfaceElement.h"
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
    //还有 “= 0”
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

TInt32 CMethod::GetFullMethodTxt(char *pszBuff,int nLength)
{
    int nRet = sprintf_s(pszBuff,nLength,"%s %s(",m_pFullRetType->m_szRawTxt.c_str(),m_szName.c_str());
    int nUsed = nRet;
    nLength -= nRet;
    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            char *pFormat;
            if (i)
            {
                pFormat = ",%s %s";
            }
            else
            {
                pFormat = "%s %s";
            }
            CParamerter *pPar = dynamic_cast<CParamerter *>(p);
            nRet = sprintf_s(pszBuff+nUsed,nLength,pFormat,pPar->m_pFullType->m_szRawTxt.c_str(),pPar->m_szName);
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }
    nRet = sprintf_s(pszBuff+nUsed,nLength,");");
    return nUsed;
}

TInt32 CMethod::GenerateStubSourceCode(char *pszBuff,int nLength)
{
    int nUsed = 0;
    int nRet = 0;
    CInterfaceElement *pIf = NULL;
    if ((m_pFather)&&(raw_interface_type == m_pFather->m_nElmentType))
    {
        pIf = dynamic_cast<CInterfaceElement*>(m_pFather);
        nRet = sprintf_s(pszBuff,nLength,"%s %s::%s(",m_pFullRetType->m_szRawTxt.c_str(),pIf->m_szName.c_str(),m_szName.c_str());
        nUsed += nRet;
        nLength -= nRet;
    }
    else
    {
        printf("Normal method not supported!");
        return -1;
    }
    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            char *pFormat;
            if (i)
            {
                pFormat = ",%s %s";
            }
            else
            {
                pFormat = "%s %s";
            }
            CParamerter *pPar = dynamic_cast<CParamerter *>(p);
            nRet = sprintf_s(pszBuff+nUsed,nLength,pFormat,pPar->m_pFullType->m_szRawTxt.c_str(),pPar->m_szName);
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }
    nRet = sprintf_s(pszBuff+nUsed,nLength,")\n{\n");
    nUsed += nRet;
    nLength -= nRet;
    
    nRet = sprintf_s(pszBuff+nUsed,nLength, "TInt32 nLen = ;//开始计算包长度"
                                            ,pIf->m_szName.c_str(),m_szFullName.c_str()); 
    nUsed += nRet;
    nLength -= nRet;
    if(m_tChilds.size())
    {
        for (int i=0;i<m_tChilds.size();++i)
        {
            CBaseElement *p = m_tChilds[i].m_pPt;
            if (raw_parameter_type == p->m_nElmentType)
            {
                char *pFormat;
                if (i)
                {
                    pFormat = "sizeof(%s)";
                }
                else
                {
                    pFormat = "+sizeof(%s)";
                }
                CParamerter *pPar = dynamic_cast<CParamerter *>(p);
                nRet = sprintf_s(pszBuff+nUsed,nLength,pFormat,pPar->m_pFullType->m_pType->m_szName.c_str());
                nUsed += nRet;
                nLength -= nRet;
            }
            else
            {
                return -1;
            }
        }
    }
    else
    {
        pszBuff[nUsed] = '0';
        ++nUsed;
        --nLength;
    }
    
    
    nRet = sprintf_s(pszBuff+nUsed,nLength, "CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(");
    nUsed += nRet;
    nLength -= nRet;
    nRet = GetMethodIdStr(pszBuff+nUsed);
    nUsed += nRet;
    nLength -= nRet;

    pszBuff[nUsed] = '|';
    pIf->GetMethodIdStr(pszBuff+nUsed);
    nUsed += nRet;
    nLength -= nRet;
    nRet = sprintf_s(pszBuff+nUsed,nLength,",&m_tTarget,1,false);\n"
                                            "if (NULL == pMsg)\n"
                                            "{\n"
                                            "return OUT_OF_MEM;\n"
                                            "}\n"
                                            "TUInt32 nUsed=0;\n"
                                            "TInt32 nRet=0;\n"
                                            "TUChar *pBuffer = pMsg->GetBody()"
                                            );
    //打包
    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = dynamic_cast<CParamerter *>(p);
            nRet = sprintf_s(pszBuff+nUsed,nLength,"nRet = Marshall(pBuffer+nUsed,%s,nLen);\n"
                                                   "if (nRet < SUCCESS)\n"
                                                   "{\n"
                                                        "return nRet;\n"
                                                   "}\n"
                                                   "nUsed += nRet;\n"
                                                   "nLen-=nRet;\n"
                                                   ,pPar->m_szName.c_str());
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }
    nRet = sprintf_s(pszBuff+nUsed,nLength,"if (nRet < SUCCESS)\n"
                                            "{\n"
                                            "return nRet;\n"
                                            "}\n"
                                            "pMsg->ResetBodyLength(nUsed);"
                                            "return m_pOnwerObj->SendMsg(pMsg);\n"
                                            "}\n"
                                             );
    nUsed += nRet;
    nLength -= nRet;
    return nUsed;
    //开始写内容
    
}

TInt32 CMethod::GetMethodIdStr(char *pBuff)
{
    char szBuff[512];
    int nRet = sprintf_s(szBuff,"%s_ID",m_szFullName.c_str());
    for (int i=0;i<nRet;++i)
    {
        szBuff[i] = toupper(szBuff[i]);
    }
    return nRet;
}
// 
// TInt32 CMethod::GeneratePresenterHeaderFile(char *pPath)
// {
//     int nRet = 0;
//     //stub 名字
//     std::string szFileName = pPath;
//     szFileName +=m_szName;
//     szFileName += "Presenter.h";
//     FILE *pFile = fopen(szFileName.c_str(),"a");
//     int nLength = 2*1024*1024;
//     char *pszBuff = NULL;
//     NEW(pszBuff,char,nLength);
//     if (!pszBuff)
//     {
//         return OUT_OF_MEM;
//     }
//     int nUsed  = 0;
// 
//     //先写头
//     CInterfaceElement *pInterface = NULL;
//     if (m_pFather)
//     {
//         if (raw_interface_type == m_pFather->m_nElmentType)
//         {
//             pInterface = dynamic_cast<CInterfaceElement *>(m_pFather);
//         }
//     }
//     if (!pInterface)
//     {
//         printf("Only interface supported!");
//         return -1;
//     }
// 
// 
//     int nBegin = 10000;
//     int n = sprintf_s(pszBuff+nBegin,1000,"__%s_%s_PRESENTER_H__",pInterface->m_szName.c_str(),m_szName.c_str());
// 
// 
// 
// 
//     for (int i=0;i<n;++i)
//     {
//         pszBuff[nBegin] = toupper(pszBuff[nBegin]);
//         ++nBegin;
//     }
// 
//     n = sprintf_s(pszBuff,nLength,"#ifndef %s \n #define %s\n",(pszBuff+10000),(pszBuff+10000));
//     nUsed += n;
//     nLength -= n;
//     
//     n = sprintf_s(pszBuff,nLength,"#include \"Public/include/TypeDef.h\"\n");
//     nUsed += n;
//     nLength -= n;
// 
//     n = sprintf_s(pszBuff+nUsed,nLength,"class %s%sPresenter \n"
//                                         "{\n"
//                                         "public:\n"
//                                         ,pInterface->m_szName.c_str(),m_szName.c_str());
//     nUsed += n;
//     nLength -= n;
//     for (int i=0;i<m_tChilds.size();++i)
//     {
//         CBaseElement *p = m_tChilds[i].m_pPt;
//         if (raw_parameter_type == p->m_nElmentType)
//         {
//             char *pFormat;
//             CParamerter *pPar = dynamic_cast<CParamerter *>(p);
//             n = sprintf_s(pszBuff+nUsed,nLength,"%s m_%s;\n",pPar->m_pFullType->m_szRawTxt.c_str(),pPar->m_szName);
//             nUsed += n;
//             nLength -= n;
//         }
//         else
//         {
//             return -1;
//         }
//     }
//     n = sprintf_s(pszBuff+nUsed,nLength,"void OnInit(");
//     nUsed += n;
//     nLength -= n;
//     
// //                                         "void OnInit()"
// //                                         "TInt32 GetLen();\n"
// //                                         "TInt32 Marshall(CMessageHeader *pMsg);\n"
// //                                         "TInt32 Unmarshall(CMessageHeader *pMsg);\n"
// //                                         "static TUInt32 GetMethodId()\"
// //                                         "{\n"
// //                                         "return ("
//                             
// 
//     nUsed += n;
//     nLength -= n;
//     n = GetMethodIdStr(pszBuff+nUsed);
//     nUsed += n;
//     nLength -= n;
//     pszBuff[nUsed] = '|';
//     ++nUsed;
//     --nLength;
//     n = pInterface->GetMethodIdStr(pszBuff+nUsed);
//     nUsed += n;
//     nLength -= n;
// 
//     n = sprintf_s(pszBuff+nUsed,nLength,");\n"
//                                         "};\n#endif\n");
//     nUsed += n;
//     nLength -= n;
//     
//     return SUCCESS;
// }
// 
// TInt32 CMethod::GeneratePresenterSourceFile(char *pPath)
// {
//     return SUCCESS;
// }


}
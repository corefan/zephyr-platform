#include "../include/Method.h"
#include "../include/Parameter.h"
#include "../include/IdlGeneratorErrorCode.h"
#include "../include/interfaceElement.h"
#include "../include/HeaderFile.h"

#pragma warning(push)
#pragma warning(disable:4018)


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
    //���� ��= 0��
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
//         else
//         {
//             OnError(ppElements[nProcess2]);
//             return -1;
//         }
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
    int nRet = sprintf(pszBuff,"virtual %s %s(",m_pFullRetType->m_szRawTxt.c_str(),m_szName.c_str());
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
            nRet = sprintf(pszBuff+nUsed,pFormat,pPar->m_pFullType->m_szRawTxt.c_str(),pPar->m_szName.c_str());
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }
    nRet = sprintf(pszBuff+nUsed,");");
    nUsed += nRet;
    //nLength-=nRet;
    return nUsed;
}

TInt32 CMethod::GetFullSkeletonMethodTxt(char *pszBuff,int nLength)
{
    return sprintf(pszBuff,"Handle%s(CMessageHeader *pMsg)",m_szFullName.c_str());
}


TInt32 CMethod::GetMethodFunPtTxt(char *pszBuff,int nLength)
{
    if ((m_pFather)&&(raw_interface_type == m_pFather->m_nElmentType))
    {
        CBaseElement *pBase = m_pFather;
        CInterfaceElement *pIf = dynamic_cast<CInterfaceElement*>(pBase);
        int nUsed = 1;
        pszBuff[0] = '{';
        --nLength;
        int n = pIf->GetMethodIdStr(pszBuff+nUsed,nLength);
        if (n > SUCCESS)
        {
            nUsed += n;
            nLength -= n;
        }
        else
        {
            return n;
        }
        n = sprintf(pszBuff+nUsed,", &%sSkeleton::Handle%s}",pIf->m_szName.c_str(),m_szFullName.c_str());
        if (n > SUCCESS)
        {
            nUsed += n;
            nLength -= n;
        }
//         else
//         {
//             return n;
//         }
        return nUsed;
        //%s|%s), &%s::Handle%s},\n",)
    }
    else
    {
        printf("Only interface method supported!!");
        return -1;
    }
    
}



TInt32 CMethod::GenerateStubSourceCode(char *pszBuff,int nLength)
{
    int nUsed = 0;
    int nRet = 0;
    CInterfaceElement *pIf = NULL;
    if ((m_pFather)&&(raw_interface_type == m_pFather->m_nElmentType))
    {
        pIf = dynamic_cast<CInterfaceElement*>(m_pFather);
        nRet = sprintf(pszBuff,"%s %sStub::%s(",m_pFullRetType->m_szRawTxt.c_str(),pIf->m_szName.c_str(),m_szName.c_str());
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
                pFormat = ",%s _%s";
            }
            else
            {
                pFormat = "%s _%s";
            }
            CParamerter *pPar = dynamic_cast<CParamerter *>(p);
            nRet = sprintf(pszBuff+nUsed,pFormat,pPar->m_pFullType->m_szRawTxt.c_str(),pPar->m_szName.c_str());
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }
    nRet = sprintf(pszBuff+nUsed,")\n{\n");
    nUsed += nRet;
    nLength -= nRet;
    
    nRet = sprintf(pszBuff+nUsed, "    TInt32 nLen = "
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
                    pFormat = "+GetLength(_%s)"; //���Զ���getLength����.
                }
                else
                {
                    pFormat = "GetLength(_%s)";
                }
                CParamerter *pPar = dynamic_cast<CParamerter *>(p);
                nRet = sprintf(pszBuff+nUsed,pFormat,pPar->m_szName.c_str());
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
    
    
    nRet = sprintf(pszBuff+nUsed, ";\n    CMessageHeader *pMsg = m_pOnwerObj->PrepareMsg(nLen,(");
    nUsed += nRet;
    nLength -= nRet;
    nRet = GetMethodIdStr(pszBuff+nUsed,nLength);
    nUsed += nRet;
    nLength -= nRet;
   
    nRet = sprintf(pszBuff+nUsed,"),&m_tTarget,1,false);\n"
                                            "    if (NULL == pMsg)\n"
                                            "    {\n"
                                            "        return OUT_OF_MEM;\n"
                                            "    }\n"
                                            "    TUInt32 nUsed=0;\n"
                                            "    TInt32 nRet=0;\n"
                                            "    TUChar *pBuffer = pMsg->GetBody();\n"
                                            );
    nUsed += nRet;
    nLength -= nRet;
    //���
    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = dynamic_cast<CParamerter *>(p);
            nRet = sprintf(pszBuff+nUsed,"    nRet = Marshall(pBuffer+nUsed,nLen,_%s);\n"
                                                   "    if (nRet < SUCCESS)\n"
                                                   "    {\n"
                                                   "        return nRet;\n"
                                                   "    }\n"
                                                   "    nUsed += nRet;\n"
                                                   "    nLen-=nRet;\n"
                                                   ,pPar->m_szName.c_str());
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }

    if (m_tChilds.size())
    {
        nRet = sprintf(pszBuff+nUsed,"    pMsg->ResetBodyLength(nUsed);\n");
        nUsed += nRet;
        nLength -= nRet;
    }
    
    nRet = sprintf(pszBuff+nUsed,"    return m_pOnwerObj->SendMsg(pMsg);\n}\n");
    nUsed += nRet;
    nLength -= nRet;
    return nUsed;
    //��ʼд����
}

TInt32 CMethod::GetMethodIdStr(char *pBuff,int nLength)
{
    int nRet = sprintf(pBuff,"%s_ID",m_szFullName.c_str());
    for (int i=0;i<nRet;++i)
    {
        pBuff[i] = toupper(pBuff[i]);
    }
    return nRet;
}
// 
// TInt32 CMethod::GeneratePresenterHeaderFile(char *pPath)
// {
//     int nRet = 0;
//     //stub ����
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
//     //��дͷ
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
//     n = sprintf(pszBuff,"#ifndef %s \n #define %s\n",(pszBuff+10000),(pszBuff+10000));
//     nUsed += n;
//     nLength -= n;
//     
//     n = sprintf(pszBuff,"#include \"Public/include/TypeDef.h\"\n");
//     nUsed += n;
//     nLength -= n;
// 
//     n = sprintf(pszBuff+nUsed,"class %s%sPresenter \n"
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
//             n = sprintf(pszBuff+nUsed,"%s m_%s;\n",pPar->m_pFullType->m_szRawTxt.c_str(),pPar->m_szName);
//             nUsed += n;
//             nLength -= n;
//         }
//         else
//         {
//             return -1;
//         }
//     }
//     n = sprintf(pszBuff+nUsed,"void OnInit(");
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
//     n = sprintf(pszBuff+nUsed,");\n"
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
TInt32 CMethod::GenerateSkeletonSourceCode(char *pszBuff,int nLength)
{
    int nUsed = 0;
    int nRet = 0;
    CInterfaceElement *pIf = NULL;
    if ((m_pFather)&&(raw_interface_type == m_pFather->m_nElmentType))
    {
        pIf = dynamic_cast<CInterfaceElement*>(m_pFather);
        nRet = sprintf(pszBuff,"%s %sSkeleton::Handle%s(CMessageHeader *pMsg)\n"
                                         "{\n"
                                         "    TInt32 nLen = pMsg->GetBodyLength();\n"
                                         ,m_pFullRetType->m_szRawTxt.c_str(),pIf->m_szName.c_str(),m_szFullName.c_str());
        nUsed += nRet;
        nLength -= nRet;
    }
    else
    {
        printf("Normal method not supported!");
        return -1;
    }
    if (m_tChilds.size())
    {
        nRet = sprintf(pszBuff+nUsed,"    TUChar *pBuffer =pMsg->GetBody();\n"
                                               "    TInt32 nRet;\n");
        nUsed += nRet;
        nLength -= nRet;
        for (int i=0;i<m_tChilds.size();++i)
        {
            CBaseElement *p = m_tChilds[i].m_pPt;
            if (raw_parameter_type == p->m_nElmentType)
            {
                CParamerter *pPar = dynamic_cast<CParamerter *>(p);
                //if (pPar->m_pFullType->IsStrPoint())
                {
                    nRet = sprintf(pszBuff+nUsed,"    %s _%s;\n",pPar->m_pFullType->m_szRawNoPrefix.c_str(),pPar->m_szName.c_str());
                }
//                 else
//                 {
//                     nRet = sprintf(pszBuff+nUsed,"    %s _%s;\n",pPar->m_pFullType->m_szRawNoPrefix.c_str(),pPar->m_szName.c_str());
//                 }
                nUsed += nRet;
                nLength -= nRet;
                nRet = sprintf(pszBuff+nUsed,"    nRet = Unmarshall(pBuffer,nLen,_%s);\n"
                                             "    if (nRet<SUCCESS)\n"
                                             "    {\n"
                                             "        pBuffer += nRet;\n"
                                             "        nLen -= nRet;\n"
                                             "    }\n"
                                             "    else\n"
                                             "    {\n"
                                             "        return nRet;\n"
                                             "    }\n"
                                            ,pPar->m_szName.c_str());
                nUsed += nRet;
                nLength -= nRet;
            }
            else
            {
                return -1;
            }
        }
    }
    nRet = sprintf(pszBuff+nUsed,"    m_pImplementObj->%s(",
                                            m_szName.c_str());
    nUsed += nRet;
    nLength-=nRet;
    
    for (int i=0;i<m_tChilds.size();++i)
    {
        CBaseElement *p = m_tChilds[i].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pPar = dynamic_cast<CParamerter *>(p);
            char *pFormat;
            if (i)
            {
                if (pPar->m_pFullType->IsPointer() && (false == pPar->m_pFullType->IsStrPoint()))//��ָ�룬���Ƿ�char*
                {
                    pFormat = ",&_%s";
                }
                else
                {
                    pFormat = ",_%s";
                }
            }
            else
            {
                if (pPar->m_pFullType->IsPointer() && (false == pPar->m_pFullType->IsStrPoint()))//��ָ�룬���Ƿ�char*
                {
                    pFormat = "&_%s";
                }
                else
                {
                    pFormat = "_%s";
                }
            }
            nRet = sprintf(pszBuff+nUsed,pFormat,pPar->m_szName.c_str());
            nUsed += nRet;
            nLength -= nRet;
        }
        else
        {
            return -1;
        }
    }

    nRet = sprintf(pszBuff+nUsed,");\n"
        "    return SUCCESS;\n"
        "}");
    nUsed += nRet;
    //nLength -= nRet;
    return nUsed;
}

TInt32 CMethod::GenerateCSharpInterfaceMethodCode(char *pBuff,int nLength)
{
    int nUsed = 0;
    int n = sprintf(pBuff,"int %s(",m_szName.c_str());
    nUsed = n;
    nLength -= n;
    for (int j=0;j<m_tChilds.size();++j)
    {
        CBaseElement *p = m_tChilds[j].m_pPt;
        if (raw_parameter_type == p->m_nElmentType)
        {
            CParamerter *pParm = (CParamerter*)p; 
            if (0 == j)
            {
                n = sprintf(pBuff+nUsed,"%s _%s",pParm->m_pFullType->GetCSharpTypeCode()->c_str(),pParm->m_szName.c_str());
            }
            else
            {
                n = sprintf(pBuff+nUsed,",%s _%s",pParm->m_pFullType->GetCSharpTypeCode()->c_str(),pParm->m_szName.c_str());
            }
            nUsed += n;
            nLength -= n;
        }
    }
    n = sprintf(pBuff+nUsed,");\n");
    nUsed += n;
    nLength -= n;
    return nUsed;
}

TInt32 CMethod::GenerateCSharpSkeletonMethodCode(char *pBuff,int nLength,int nEtchNr)
{
    int nUsed = 0;
    int n = 0;
    WRITE_LINE_ETCH("int %s(CMessage pMsg)",m_szFullName.c_str());
    WRITE_LINE_ETCH("{");
    ++nEtchNr;
    WRITE_LINE_ETCH("int nUsed = MacrosAndDef.MSG_HEADER_LEN;");
    WRITE_LINE_ETCH("int nBufferLen = pMsg.m_pBuffers.Length;");

    for(int i=0;i<m_tChilds.size();++i)
    {
        //call

        WRITE_LINE_ETCH("if (nLen < MacrosAndDef.SUCCESS)");
        WRITE_LINE_ETCH("{");
        ++nEtchNr;
        WRITE_LINE_ETCH("return MacrosAndDef.OUT_OF_MEM;");
        --nEtchNr;
        WRITE_LINE_ETCH("{");
        WRITE_LINE_ETCH("nUsed += nLen;");
        //return
    }
    --nEtchNr;
    WRITE_LINE_ETCH("}");
    
    return nUsed;
}

}
#pragma warning(pop)

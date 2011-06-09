#include "../include/HeaderFile.h"
#include "../include/interfaceElement.h"
#include "../include/FullTypeClass.h"
#include "../include/IdlGeneratorErrorCode.h"
#include "../include/Method.h"
#include "../include/Parameter.h"
#include <vector>


namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CInterfaceElement)


CInterfaceElement::CInterfaceElement()
{
    m_nElmentType = raw_interface_type;
}


CInterfaceElement::EnState CInterfaceElement::GetState(char *pAlphabets)
{
    return en_class_key_word;
}

TInt32 CInterfaceElement::Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles)
{
    EnState enLastState = en_class_key_word;
    EnType enterAndDividerTypes[2] = {enter_type,divider_type};
    struct Tt
    {
        char *m_pszTxt;
        EnState   m_enType;
    };
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
                                enLastState = en_class_right_brace;
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
                        enLastState = en_class_right_brace;
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
        case         en_class_right_brace:     //{ 这个时候需要的是一个type类型，或者 virtual \ pubic \ protected \private \ class \enum \struct \const \static \volatile \mutal 
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
                            case key_class:
                                {
                                    //新的 class
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
            } //case         en_class_right_brace:
            break;
        }
    }
    
    if (semicolon_type == pTypes[nProcess2])
    {
        return nProcess2 - nOld + 1;
    }
    return 0;
}


int CInterfaceElement::HandleAStatement(char **ppElements,EnType *pTypes,int& nProcess2,int nTotalEles)
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
        if (blanket_type_1 == pTypes[nProcess2])
        {
            CMethod *pMethod = CREATE_FROM_STATIC_POOL(CMethod);
            if (!pMethod)
            {
                char *pAt =NULL;
                if (nProcess2 < nTotalEles)
                {
                    pAt = ppElements[nTotalEles];
                }
                OnError(pAt);
                return OUT_OF_MEM;
            }
            nRet = pMethod->Process(ppElements,pTypes,nProcess2,nTotalEles);
            if (nRet > SUCCESS)
            {
                pMethod->SetName(pName);
                pMethod->SetRtnType(pFullType);
                AddChildElement(pMethod);
                nProcess2 += nRet;
                return 1;
            }
            //肯定是
            OnError(ppElements[nProcess2]);
            return -1;
        }
        else
        {
            //处理数组？ 
            printf("Find a menber of the class!");
            while(semicolon_type != pTypes[nProcess2])
            {
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

const char *CInterfaceElement::GetHierachyName(void)
{
    return m_szName.c_str();
}

}
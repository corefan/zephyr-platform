#include "../include/HeaderFile.h"
#include "../include/interfaceElement.h"
#include <vector>


namespace Zephyr
{

IMPLEMENT_STATIC_CLASS_POOL(CInterfaceElement)


void CInterfaceElement::OnError(int nProcess2)
{
    return;
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
                    OnError(nProcess2);
                    return -1;
                }
            }
            break;
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
                                OnError(nProcess2);
                                return -1;
                            }
                        }
                        else
                        {
                            OnError(nProcess2);
                            return -1;
                        }
                    }
                }
                else
                {
                    OnError(nProcess2);
                    return -1;
                }
            }
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
                    OnError(nProcess2);
                    return -1;
                }
            }
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
                    OnError(nProcess2);
                    return -1;
                }
            }
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
                        OnError(nProcess2);
                        return -1;
                    }
                }
            }
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
                            if (operator_type == pTypes[nProcess2])
                            {
                                if (ppElements[nProcess2][0]==';')//结束了
                                {
                                    ++nProcess2;
                                    return nProcess2;
                                }
                            }
                            else
                            {
                                OnError(nProcess2);
                                return FAIL;
                            }
                        }
                        else if (ppElements[nProcess2][0]=='~')
                        {
                            //处理析构函数
                        }
                        else
                        {
                            OnError(nProcess2);
                            return FAIL;
                        }
                       
                    }
                    break;
                case alphabet_type:
                    {
                        //if ()
                        CBaseElement *pEle = m_pOwner->IsKeyWords(ppElements[nProcess2]);
                        if (pEle)
                        {
                            switch (pEle->m_nElmentType)
                            {
                            case key_static: //相同处理
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
                                        OnError(nProcess2);
                                        return OUT_OF_MEM; 
                                    }
                                    ++nProcess2;
                                }
                                break;
                            case key_namespace   :
                                {
                                    //不该有
                                    OnError(nProcess2);
                                    return FAIL;
                                }
                                break;
                            case key_struct      :
                                {
                                    //暂时不处理   
                                    OnError(nProcess2);
                                    return FAIL;
                                }
                                break;
                            case key_const       :
                                {
                                    //pHeap.push_back();
                                }
                                break;
                            case key_volatile    :
                                {

                                }
                                break;
                            case key_mutable     :
                                {

                                }
                                break;
                            case key_public      :
                                {

                                }
                                break;
                            case key_protected   :
                                {

                                }
                                break;
                            case key_private     :
                                {

                                }
                                break;
                            case key_include     :
                                {

                                }
                                break;
                            case key_pragma      :
                                {

                                }
                                break;
                            case key_nr_define      :
                                {

                                }
                                break;
                            case key_nr_ifdef       :
                                {

                                }
                                break;
                            case key_nr_ifndef      :
                                {

                                }
                                break;
                            case key_nr_endif       :
                                {

                                }
                                break;
                            case key_nr_else        :
                                {

                                }
                                break;
                            case key_while       :
                                {

                                }
                                break;
                            case key_if          :
                                {

                                }
                                break;
                            case key_else        :
                                {

                                }
                                break;
                            case key_for         :
                                {

                                }
                                break;
                            case key_break       :
                                {

                                }
                                break;
                            case key_continue    :
                                {

                                }
                                break;
                            case key_goto        :
                                {

                                }
                                break;
                            case key_switch      :
                                {

                                }
                                break;
                            case key_extern      :
                                {

                                }
                                break;
                            }
                        }

                    }
                }
            }
            break;
        case  en_class_public_key_word:
            {

            }
            break;
        case  en_class_public_key_risk:
            {

            }
            break;
        case en_class_protected_key_word:
            {

            }
            break;
        case en_class_protected_key_risk:
            {

            }
            break;
        case en_class_private_key_word:
            {

            }
            break;
        case en_class_private_key_risk:
            {

            }
            break;
        case en_class_type:
            {

            }
            break;
        case en_class_type_operator:
            {

            }
            break;
        case en_class_typeName:
            {

            }
            break;
        case en_class_typeName_right_bracket:
            {

            }
            break;
                //函数定义
        case en_class_typeName_semicolon:
            {

            }
            break;
        case en_class_friend_key_word:
            {

            }
            break;
        case en_class_left_brace:
            {

            }
            break;
        }
    }
    
    if (0 == strcmp(ppElements[nProcess2],";"))
    {
        return nProcess2 - nOld + 1;
    }
    return 0;
}

const char *CInterfaceElement::GetHierachyName(void)
{
    return m_szName.c_str();
}

}
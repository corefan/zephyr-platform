#include "../include/interfaceElement.h"

namespace Zephyr
{

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
    while (nProcess2 < nTotalEles)
    {
        //忽律所有

        int nRet = IgnorType(ppElements,pTypes,nProcess2,nTotalEles,divider_type);
        nProcess2 += nRet;
        switch (enLastState)
        {
        case en_class_key_word:
            {
                if (pTypes[nProcess2] == alphabet_type)
                {
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
                        
                        int nRet = IgnorTypes(ppElements,pTypes,nProcess2,nTotalEles,2,enterAndDividerTypes);
                        nProcess2 += nRet;
                        if (pTypes[nProcess2] == operator_type)
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
                        //derived class
                        enLastState = en_class_derive_class_name;
                        ++nProcess2;
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
                int nRet = IgnorTypes(ppElements,pTypes,nProcess2,nTotalEles,2,enterAndDividerTypes);
                nProcess2 += nRet;
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
        case         en_class_right_brace:     //{
            {
                int nRet = IgnorType(ppElements,pTypes,nProcess2,nTotalEles,enter_type);
                switch(pTypes[nProcess2])
                {
                case operator_type:
                    {
                        if (ppElements[nProcess2][0]=='}')//结束了
                        {
                            ++nProcess2;
                            return nProcess2;
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
                    }
                }
            }
            break;
        case         en_class_public_key_word:
            {

            }
            break;
        case         en_class_public_key_risk:
            {

            }
            break;
        case         en_class_protected_key_word:
            {

            }
            break;
        case         en_class_protected_key_risk:
            {

            }
            break;
        case         en_class_private_key_word:
            {

            }
            break;
        case         en_class_private_key_risk:
            {

            }
            break;
        case         en_class_type:
            {

            }
            break;
        case         en_class_type_operator:
            {

            }
            break;
        case         en_class_typeName:
            {

            }
            break;
        case         en_class_typeName_right_bracket:
            {

            }
            break;
                //函数定义
        case         en_class_typeName_semicolon:
            {

            }
            break;
        case         en_class_friend_key_word:
            {

            }
            break;
        case         en_class_left_brace:
            {

            }
            break;
        }
    }
    
    if (0 == strcmp(ppElements[nProcess2],"virtual"))
    {
        
    }
    return 0;
}


}
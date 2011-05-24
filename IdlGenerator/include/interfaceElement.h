/*-------------------------------------------------------------
 Copyright (C)| 
 File: interfaceElement.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 编译使用的接口类
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_INTERFACE_ELEMENT_H__
#define __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_INTERFACE_ELEMENT_H__

#include "baseElement.h"


class CHeaderFile;
namespace Zephyr
{

class CInterfaceElement : public CBaseElement
{
public:
    DECLARE_STATIC_CLASS_POOL(CInterfaceElement);
private:
    CHeaderFile *m_pOwner;

    enum EnState
    {
        en_class_key_word,    //class
        en_class_name,        //CName
        en_class_name_risk,   //:
        en_class_derive_virtual, //virtual 
        en_class_derive_class_name, //CDerive 
        en_class_right_brace,     //{
        en_class_public_key_word,
        en_class_public_key_risk,
        en_class_protected_key_word,
        en_class_protected_key_risk,
        en_class_private_key_word,
        en_class_private_key_risk,
        en_class_type,
        en_class_type_operator,
        en_class_typeName,
        en_class_typeName_right_bracket,
        //函数定义
        en_class_typeName_semicolon,
        en_class_friend_key_word,
        en_class_left_brace,
    };
public:
    //class Name {  public : method1 method2 ...} ;
    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);
    virtual const char *GetHierachyName(void);
    void OnError(int nProcess2);
    EnState GetState(char *pAlphabets);
};

}

#endif

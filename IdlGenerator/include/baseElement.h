/*-------------------------------------------------------------
 Copyright (C)| 
 File: baseElement.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 编译使用的最小属性
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_BASE_ELEMENT_H__
#define __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_BASE_ELEMENT_H__

#include "../../Public/include/TypeDef.h"
#include "../../Public/include/SysMacros.h"
#include <string>

using namespace std;
namespace Zephyr
{


enum EnEleType
{
    en_interface_type,
    en_class_type,
    en_enum_type,
    en_base_c_type,
    en_include_type,
    en_define_type,
    en_struct_type,
    en_union_type,
    en_divider_type,
    en_operator_type,
};

enum EnType
{
    not_acceptable_type = 0,
    enter_type          = 1,
    divider_type        = 2,
    operator_type       = 4,
    alphabet_type       = 5,
    num_type            = 6,
};

class CBaseElement
{
public:
    string m_szName; //
    int     m_nElmentType;

    string &GetKey()
    {
        return m_szName;
    }
public:
    virtual ~CBaseElement()
    {

    }
    //处理下一个字符组
    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);

    TInt32 IgnorType(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles,EnType eType);
    TInt32 IgnorTypes(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles,int nrOfType,EnType eType[]);
};
}



#endif

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

class CBaseElement
{
public:
    string m_szName; //
    int     m_nElmentType;
    string &GetKey()
    {
        return m_szName;
    }
};

}



#endif

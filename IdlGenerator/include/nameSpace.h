/*-------------------------------------------------------------
 Copyright (C)| 
 File: method.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 编译使用的namespace,namespace 和类唯一的不同是，不能写public\protected\private关键词
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/


#ifndef __ZEPHYR_IDL_GENERATOR_NAME_SPACE_H__
#define __ZEPHYR_IDL_GENERATOR_NAME_SPACE_H__
#include "./baseElement.h"

namespace Zephyr
{


class CNamespace : public CBaseElement
{
public:
    DECLARE_STATIC_CLASS_POOL(CNamespace);
public:
    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);

    //肯定是 Type + [&|*] + name +['('+[Type+[*|&]+[NAME+[=+(Num|Str|Char)]]]+[,+TYPE+[&|*]+[NAME]]+')'] + ';'
    //Type =  [const|volatile|static]+[(NAMESPACE|CLASSNAME)+::]+name
    int findAType(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);//Type;
    int findAComma(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles); //,
    int findAName(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles); //name;
    int findTypeOpr(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);//[*|**|*&|&|**];
    int findMethod(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles); //[(+[Type+[*|&]+[NAME+[=[Num|Str|Char]]]]+[,+TYPE+[&|*]+[NAME]])];
    int findADefaultValue(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles); //[=[Num|Str|Char]];
    

};

}

#endif

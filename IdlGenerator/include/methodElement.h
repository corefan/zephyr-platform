/*-------------------------------------------------------------
 Copyright (C)| 
 File: methodElement.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 编译使用的接口类
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_METHOD_ELEMENT_H__
#define __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_METHOD_ELEMENT_H__

#include "baseElement.h"
#include "../../Public/tpl/include/TplList.h"
#include "../../Public/tpl/include/TplPool.h"

namespace Zephyr
{

class CMethodElement :public CBaseElement
{
protected:
    CBaseElement *m_pRetValueType;
    CList<CMethodParameter> m_tElements;
public:
    CBaseElement *GetRetValueType();
    int           GetParameterNr();


};

}

#endif

/*-------------------------------------------------------------
 Copyright (C)| 
 File: method.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 编译使用的方法的类
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_METHOD_ELEMENT_H__
#define __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_METHOD_ELEMENT_H__

#include "baseElement.h"
#include "../../Public/tpl/include/TplPool.h"

namespace Zephyr
{

class CMethod : public CBaseElement
{
protected:
    
    CBaseElement    *m_pTypePointer;

public:
    std::string &GetParameterName()
    {
        return m_tParameterName;
    }
//     CMethodElement *GetMethod()
//     {
//         return m_pBelongs2;
//     }


    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);
};

}

#endif


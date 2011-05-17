/*-------------------------------------------------------------
 Copyright (C)| 
 File: methodParamterElement.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 编译使用的接口类
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_METHOD_PARAMETER_ELEMENT_H__
#define __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_METHOD_PARAMETER_ELEMENT_H__

#include "baseElement.h"
#include "../../Public/tpl/include/TplPool.h"

namespace Zephyr
{

class CMethodParameter : public CBaseElement
{
protected:
    
    std::string     m_tParameterName;

    CBaseElement    *m_pTypePointer;
    CMethodParameter *m_pBelongs2;

    int             m_nParameterIdx;
public:
    std::string &GetParameterName()
    {
        return m_tParameterName;
    }
//     CMethodElement *GetMethod()
//     {
//         return m_pBelongs2;
//     }
    int             GetIdx()
    {
        return m_nParameterIdx;
    }
    void Init(const char *pName,CBaseElement *pBaseElement, CMethodParameter *pBelongs2,int nIdx);

    virtual int Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles) = 0;
};

}

#endif


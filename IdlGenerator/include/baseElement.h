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
#include "../../Public/tpl/include/TplPool.h"
#include "../../Public/tpl/include/tplmap.h"
#include <string>
#include <vector>

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
    not_acceptable_type  = 0,
    enter_type              ,
    divider_type            ,
    operator_type           ,
    slash_mark_type         ,
    back_slash_mark_type    ,
    star_mark_type          ,
    single_quotes_mark_type ,
    quotes_mark_type        ,
    alphabet_type           ,
    num_type                ,
    const_string            ,
    const_char              ,
    blanket_type_1          , //'('
    blanket_type_2          , //')'
};

enum EnKeyWords
{
    key_class       ,
    key_namespace   ,
    key_struct      ,
    key_static      ,
    key_const       ,
    key_volatile    ,
    key_mutable     ,
    key_public      ,
    key_protected   ,
    key_private     ,
    key_include     ,
    key_pragma      ,
    key_nr_define      ,
    key_nr_ifdef       ,
    key_nr_ifndef      ,
    key_nr_endif       ,
    key_nr_else        ,
    key_while       ,
    key_if          ,
    key_else        ,
    key_for         ,
    key_break       ,
    key_continue    ,
    key_goto        ,
    key_switch      ,
    key_extern      ,
};


template <class T,class KEY>
class TplPtPack
{
public:
    TplPtPack()
    {
        m_pPt = NULL;
    }
    T   *m_pPt;
    T& operator->()
    {
        return *m_pPt;
    }
    KEY &GetKey()
    {
        return m_pPt->GetKey();
    }
};


class CBaseElement
{
public:

    //static CPool<CBaseElement>  sm_tPool; 
    //static TInt32 InitPool(TInt32 nPoolSize);

    DECLARE_STATIC_CLASS_POOL(CBaseElement) ;

    static TplMap<TplPtPack<CBaseElement,string >,string> *sm_pBaseElements;

    static TplMap<TplPtPack<CBaseElement,string >,string> *sm_pBaseKeyWords;

    string m_szName; //
    int     m_nElmentType;
    CBaseElement *m_pFather;

    struct CInstance
    {
        TplPtPack<CBaseElement,string> m_tType;
        enum EnSubType
        {
            en_orignal_type,
            en_member_type,
            en_pointer_type,
            en_pointer_pointer_type,
            en_pointer_p_p_type,
            en_ref_type,
            en_pointer_ref_type,
            en_pointer_p_ref_type,
        };

        char                          m_nSubType[4];
        string                        m_szInstanceName;

    };
    vector<CInstance> m_tChilds;
    string &GetKey()
    {
        return m_szName;
    }

public:
    CBaseElement()
    {
        m_nElmentType = 0;
        m_pFather = NULL;
    }
    virtual ~CBaseElement()
    {

    }

    void SetName(const char* pName)
    {
        m_szName = pName;
    }
    const char* GetName()
    {
        return m_szName.c_str();
    }
    //处理下一个字符组
    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);
    virtual const char *GetHierachyName();

    TInt32 IgnorType(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles,EnType eType);
    TInt32 IgnorTypes(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles,int nrOfType,EnType eType[]);

    static TInt32   AddType(CBaseElement *pBaseElement);
    static TInt32   AddKeyWords(const char *pName,EnKeyWords key);
    static CBaseElement *IsKeyWords(const char *psz);

    void   AddChildElement(CBaseElement *pElement,const char *pSubType,const char* pszName);
    void MakeOneWords(char **ppWords,int nFrom,int nWordsNr);

    bool IsBracesBegin(char *pStr,EnType nType);
    bool IsBracesEnd(char *pStr,EnType nType);
};

}



#endif

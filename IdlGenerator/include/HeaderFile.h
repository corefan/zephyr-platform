/*-------------------------------------------------------------
 Copyright (C)| 
 File: HeaderFile.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 一个文件
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_HEADER_FILE_H__
#define __ZEPHYR_SERVICE_CONTANER_IDL_GENERATOR_HEADER_FILE_H__
#include "../../Public/include/TypeDef.h"
#include "./baseElement.h"
#include "../../Public/tpl/include/tplmap.h"

namespace Zephyr
{
    
class CHeaderFile
{
public:
    static TplMap<CBaseElement,string> *sm_pBaseElements;

    static TplMap<CBaseElement,string> *sm_pBaseKeyWords;
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
protected:
    CBaseElement **m_ppElements;
    char         **m_ppWords;
    EnType         *m_pWordsTypes;
    char          *m_pszFileInPiece;
    TUInt32       m_nNrOfWords;
    TUInt32       m_nNrOfElements;
    TUInt32       m_nFileLength;
    char    *m_pszFile;
    std::string   m_szFileName;
public:
    CHeaderFile();
    ~CHeaderFile();
    TInt32 GeneratorIdl(const char *pFileName);
private:
    //读取文件
    TInt32 ReadFile(const char *pFileName);
    //分词
    TInt32 DividIntoWords();
    //第一次语法分析,确定每个词的类型
    TInt32 SyntaxCompile1st();
    //类型组合
    TInt32 SyntaxCompile2nd();
    //生成Idl代码
    TInt32 GeneratorIdlCode();

    TBool  IsDivider(char c);
    TBool  IsEnter(char c);
    TBool  IsOperator(char c);
    TBool  IsAlphabet(char c);
    TBool  IsNum(char c);
    static TInt32   AddType(const char*pName);
    static TInt32   AddKeyWords(const char *pName,EnKeyWords key);
};

}

#endif

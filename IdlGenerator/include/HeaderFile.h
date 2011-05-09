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


namespace Zephyr
{




class CHeaderFile
{
protected:
    CBaseElement **m_ppElements;
    char         **m_ppWords;
    TUInt32       m_nNrOfWords;
    TUInt32       m_nNrOfElements;
    const char    *m_pszFile;
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
};

}

#endif

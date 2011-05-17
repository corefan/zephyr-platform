#include "../include/HeaderFile.h"

namespace Zephyr
{


CHeaderFile::CHeaderFile()
{
    m_ppElements = NULL;
    m_ppWords = NULL;
    m_nNrOfWords = 0;
    m_nNrOfElements = 0;
    m_pszFile = NULL;
    if (NULL == sm_pBaseElements)
    {
        sm_pBaseElements = new TplMap<CBaseElement,string>;
        CPool<TplNode<CBaseElement,string> > *pPool = new CPool<TplNode<CBaseElement,string> >;
        pPool->InitPool(100);
        sm_pBaseElements = new TplMap<CBaseElement,string>;
        sm_pBaseElements->Init(pPool);
    }
    if (NULL == sm_pBaseKeyWords)
    {
        sm_pBaseElements = new TplMap<CBaseElement,string>;
        CPool<TplNode<CBaseElement,string> > *pPool = new CPool<TplNode<CBaseElement,string> >;
        pPool->InitPool(100);
        sm_pBaseKeyWords = new TplMap<CBaseElement,string>;
        sm_pBaseKeyWords->Init(pPool);

        AddKeyWords("class",key_class);
        AddKeyWords("namespace", key_namespace);
        AddKeyWords("struct",key_struct);
        AddKeyWords("static",key_static);
        AddKeyWords("const",key_const );
        AddKeyWords("volatile",key_volatile     );
        AddKeyWords("mutable",key_mutable      );
        AddKeyWords("public",key_public       );
        AddKeyWords("protected",key_protected    );
        AddKeyWords("private",key_private      );
        AddKeyWords("include",key_include      );
        AddKeyWords("pragma",key_pragma       );
        AddKeyWords("nr_define",key_nr_define       );
        AddKeyWords("nr_ifdef",key_nr_ifdef        );
        AddKeyWords("nr_ifndef",key_nr_ifndef       );
        AddKeyWords("nr_endif",key_nr_endif        );
        AddKeyWords("nr_else",key_nr_else         );
        AddKeyWords("while",key_while        );
        AddKeyWords("if",key_if           );
        AddKeyWords("else",key_else         );
        AddKeyWords("for",key_for          );
        AddKeyWords("break",key_break        );
        AddKeyWords("continue",key_continue     );
        AddKeyWords("goto",key_goto         );
        AddKeyWords("switch",key_switch       );
        AddKeyWords("extern",key_extern       );
    }
}

CHeaderFile::~CHeaderFile()
{

}

TplMap<CBaseElement,string> *CHeaderFile::sm_pBaseElements = NULL;
TplMap<CBaseElement,string> *CHeaderFile::sm_pBaseKeyWords = NULL;

TInt32   CHeaderFile::AddType(const char*pName)
{
    CBaseElement *pItem = sm_pBaseElements->PrepareItem();
    if (pItem)
    {
        pItem->m_szName = pName;
        int nRet = sm_pBaseElements->AddInTree(pItem);
        if (nRet < SUCCESS)
        {
            sm_pBaseElements->ReleaseItem(pItem);
            return nRet;
        }
        return SUCCESS;
    }
    return OUT_OF_MEM;
}
TInt32   CHeaderFile::AddKeyWords(const char *pName,EnKeyWords key)
{
    CBaseElement *pItem = sm_pBaseKeyWords->PrepareItem();
    if (pItem)
    {
        pItem->m_szName = pName;
        pItem->m_nElmentType = key;
        int nRet = sm_pBaseKeyWords->AddInTree(pItem);
        if (nRet < SUCCESS)
        {
            sm_pBaseKeyWords->ReleaseItem(pItem);
            return nRet;
        }
        return SUCCESS;
    }
    return OUT_OF_MEM;
}

TInt32 CHeaderFile::GeneratorIdl(const char *pFileName)
{
    ReadFile(pFileName);
    DividIntoWords();
    return SUCCESS;
}

    //读取文件,调用者保证pFileName的可用性
TInt32 CHeaderFile::ReadFile(const char *pFileName)
{
    FILE *pFile = fopen(pFileName,"r");
    if (pFile)
    {
        fseek(pFile,0,SEEK_END);
        m_nFileLength = ftell(pFile);
        if (m_nFileLength)
        {
            m_pszFile = new char[m_nFileLength];
            rewind(pFile);

            int result = fread (m_pszFile,1,m_nFileLength,pFile);
            if (result < m_nFileLength) 
            {
                m_nFileLength = result;
            }
        }
        fclose (pFile);
    }
    return SUCCESS;
}
    //分词
TInt32 CHeaderFile::DividIntoWords()
{
    EnType cTypes[256];
    char c = 0;
    for (int i=0;i<256;++i)
    {
        if (IsEnter(c))
        {
            cTypes[i] = enter_type;
        }
        else if (IsDivider(c))
        {
            cTypes[i] = divider_type;
        }
        else if (IsOperator(c))
        {
            cTypes[i] = operator_type;
        }
        else if (IsAlphabet(c))
        {
            cTypes[i] = alphabet_type;
        }
        else if (IsNum(c))
        {
            cTypes[i] = num_type;
        }
        else
        {
            cTypes[i] = not_acceptable_type;
        }
        ++c;
    }
    //空格 0 符号 1 字母 2 数字 3 
    EnType nLastType = divider_type;
    int nLastWordIdx = 0;
    m_pszFileInPiece = new char[m_nFileLength*2];
    memset(m_pszFileInPiece,0,(m_nFileLength*2));
    m_ppWords = new char*[m_nFileLength/2];
    m_pWordsTypes = new EnType[m_nFileLength/2];
    memset(m_ppWords,0,(sizeof(char*)*(m_nFileLength/2)));
    m_nNrOfWords = 0;
    char *pRead2 = m_pszFileInPiece;
    nLastType =(EnType)cTypes[m_pszFile[0]];

    for (int i = 1;i<m_nFileLength;++i)
    {
        unsigned char *pCur = (unsigned char*)&m_pszFile[i];
        switch (nLastType)
        {
        case enter_type:
        case divider_type:
            {
                switch (cTypes[(unsigned int)(*pCur)])
                {
                case enter_type:
                case divider_type:
                    {
                        //继续
                    }
                    break;
                case operator_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;

                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = operator_type;
                    }
                    break;
                case alphabet_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = alphabet_type;
                    }
                    break;
                case num_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = num_type;
                    }
                    break;
                default:
                    {
                        if (((unsigned int)(*pCur))>0x80)
                        {
                            if (i < (m_nFileLength-1))
                            {
                                if (((unsigned int)m_pszFile[i+1])>0x80) //是个中文字符？！
                                {
                                    memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                                    pRead2[(i-nLastWordIdx)] = 0;
                                    m_ppWords[m_nNrOfWords] = pRead2;
                                    m_pWordsTypes[m_nNrOfWords] = nLastType;
                                    ++m_nNrOfWords;
                                    pRead2 += ((i-nLastWordIdx)+1);
                                    nLastWordIdx = i;
                                    ++i; //再跳过一个字符
                                    nLastType = alphabet_type;
                                    break;
                                }
                            }
                        }
                        printf("find unacceptable char:%d",((int)m_pszFile[i]));
                        return -1;
                    }
                }
            }
            break;
        case operator_type:
            {
                switch (cTypes[(unsigned int)(*pCur)])
                {
                case enter_type:
                case divider_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = cTypes[(unsigned int)(*pCur)];
                    }
                    break;
                case operator_type:
                    {
                        //继续
                    }
                    break;
                case alphabet_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = alphabet_type;
                    }
                    break;
                case num_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = num_type;
                    }
                    break;
                default:
                    {
                        if (((unsigned int)(*pCur))>0x80)
                        {
                            if (i < (m_nFileLength-1))
                            {
                                if (((unsigned int)m_pszFile[i+1])>0x80) //是个中文字符？！
                                {
                                    memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                                    pRead2[(i-nLastWordIdx)] = 0;
                                    m_ppWords[m_nNrOfWords] = pRead2;
                                    m_pWordsTypes[m_nNrOfWords] = nLastType;
                                    ++m_nNrOfWords;
                                    pRead2 += ((i-nLastWordIdx)+1);
                                    nLastWordIdx = i;
                                    ++i; //再跳过一个字符
                                    nLastType = alphabet_type;
                                    break;
                                }
                            }
                        }
                        printf("find unacceptable char:%d",((int)m_pszFile[i]));
                        return -1;
                    }
                }
            }
            break;
        case alphabet_type:
            {
                switch (cTypes[(unsigned int)(*pCur)])
                {
                case enter_type:
                case divider_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = cTypes[(unsigned int)(*pCur)];
                    }
                    break;
                case operator_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = operator_type;
                    }
                    break;
                case alphabet_type:
                    {
                        //continue
                    }
                    break;
                case num_type:
                    {
                        //continue 
                    }
                    break;
                default:
                    {
                        if (((unsigned int)(*pCur))>0x80)
                        {
                            if (i < (m_nFileLength-1))
                            {
                                if (((unsigned int)pCur[1])>0x80) //是个中文字符？！
                                {
                                    ++i; //再跳过一个字符
                                    break;
                                }
                            }
                        }
                        printf("find unacceptable char:%d",((int)m_pszFile[i]));
                        return -1;
                    }
                }
            }
            break;
        case num_type:
            {
                switch (cTypes[(unsigned int)(*pCur)])
                {
                case enter_type:
                case divider_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = cTypes[(unsigned int)(*pCur)];
                    }
                    break;
                case operator_type:
                    {
                        memcpy(pRead2,m_pszFile+nLastWordIdx,i-nLastWordIdx);
                        pRead2[(i-nLastWordIdx)] = 0;
                        m_ppWords[m_nNrOfWords] = pRead2;
                        m_pWordsTypes[m_nNrOfWords] = nLastType;
                        ++m_nNrOfWords;
                        pRead2 += ((i-nLastWordIdx)+1);
                        nLastWordIdx = i;
                        nLastType = operator_type;
                    }
                    break;
                case alphabet_type:
                    {
                        //继续
                        nLastType = alphabet_type;
                    }
                    break;
                case num_type:
                    {
                        //继续
                    }
                    break;
                default:
                    {
                        if (((unsigned int)(*pCur))>0x80)
                        {
                            if (i < (m_nFileLength-1))
                            {
                                if (((unsigned int)(*(pCur+1)))>0x80) //是个中文字符？！
                                {
                                    ++i; //再跳过一个字符
                                    nLastType = alphabet_type;
                                    break;
                                }
                            }
                        }
                        printf("find unacceptable char:%d",((int)m_pszFile[i]));
                        return  -1;
                    }
                }
            }
            break;
        default:
            {
                printf("Find incoorect type!");
                return -1;
            }
        }
    }
    for (int i=0;i<m_nNrOfWords;++i)
    {
        printf("%s ",m_ppWords[i]);
    }
    return SUCCESS;
}

TBool  CHeaderFile::IsDivider(char c)
{
    switch(c)
    {
    case ' ':
        {
        }
        break;
    case '\t':
        {
        }
        break;
    default:
        {
            return FALSE;
        }
    }
    return TRUE;
}

TBool CHeaderFile::IsEnter(char c )
{
    if ('\n' == c)
    {
        return TRUE;
    }
    return FALSE;
}

TBool CHeaderFile::IsOperator(char c)
{

    switch(c)
    {
    case ';':
        {

        }
        break;
        case '\'':
        {

        }
        break;
    case '\"':
        {

        }
        break;
    case '*':
        {

        }
        break;
    case '&':
        {

        }
        break;
    case '+':
        {

        }
        break;
    case '-':
        {

        }
        break;
    case '=':
        {
        }
        break;
    case '{':
        {
        }
        break;
    case '}':
        {
            
        }
        break;
    case '!':
        {
        }
        break;
    case '(':
        {
        }
        break;
    case ')':
        {
        }
        break;
    case '[':
        {
        }
        break;
    case ']':
        {
            
        }
        break;
    case '~':
        {

        }
        break;
    case '#':
        {

        }
        break;
    case '\\':
        {

        }
        break;
    case '/':
        {
            
        }
        break;
    case ',':
        {

        }
        break;
    case '|':
        {

        }
        break;
    case '?':
        {

        }
        break;
    case '.':
        {

        }
        break;
    case '>':
        {

        }
        break;
    case '<':
        {

        }
        break;
    case ':':
        {

        }
        break;
    case '%':
        {

        }
        break;
    case '^':
        {

        }
        break;
    default:
        {
            return FALSE;
        }
    }
    return TRUE;
}

TBool  CHeaderFile::IsAlphabet(char c)
{
    if ((c>='a')&&(c<='z'))
    {
        return TRUE;
    }
    if ((c>='A')&&(c<='Z'))
    {
        return TRUE;
    }
    if (c == '_')
    {
        return TRUE;
    }
    return FALSE;
}
TBool  CHeaderFile::IsNum(char c)
{
    if ((c>='0')&&(c<='9'))
    {
        return TRUE;
    }
    return FALSE;
}

    //第一次语法分析,确定每个词的类
TInt32 CHeaderFile::SyntaxCompile1st()
{
    return SUCCESS;
}
    //类型组合
TInt32 CHeaderFile::SyntaxCompile2nd()
{
    return SUCCESS;
}
    //生成Idl代码
TInt32 CHeaderFile::GeneratorIdlCode()
{
    return SUCCESS;
}

}

#ifndef __ZEPHYR_IDL_GENERATOR_STRUCT_H__
#define __ZEPHYR_IDL_GENERATOR_STRUCT_H__

#include "../../Public/include/TypeDef.h"
#include "baseElement.h"
#include "../../Public/tpl/include/TplPool.h"
#include "./FullTypeClass.h"
namespace Zephyr
{

class CHeaderFile;
class CStruct : public CBaseElement
{
public:
    CHeaderFile *m_pOwner;
    enum EnState
    {
        en_class_key_word,    //class
        en_class_name,        //CName
        en_class_name_risk,   //:
        en_class_derive_virtual, //virtual 
        en_class_derive_class_name, //CDerive 
        en_class_left_brace,
        en_class_right_brace,     //
    };

public :
    DECLARE_STATIC_CLASS_POOL(CStruct);
    CStruct()
    {
        m_nElmentType = raw_struct_type;
        m_pOwner = NULL;
    }
    virtual TInt32 Process(char **ppElements,EnType *pTypes,int nProcess2,int nTotalEles);
    virtual const char *GetHierachyName();
    virtual TInt32 GenerateSkeleton(const char *pPath);
    virtual TInt32 GenerateStub(const char *pPath);
    virtual TInt32 GenerateMethodId(const char *pPath); //生成MarshallAll.h
    virtual TInt32 GenerateCSharpCode(const char *pPath,int nChoice);
    int HandleAStatement(char **ppElements,EnType *pTypes,int &nProcess2,int nTotalEles);

private:
    TInt32 GenerateStubHeaderFile(const char *pPath); //生成Marshaller.h
    TInt32 GenerateStubSourceFile(const char *pPath); //生成Marshaller.cpp
    TInt32 GenerateSkeletonHeaderFile(const char *pPath); //生成UnMarshaller.h
    TInt32 GenerateSkeletonSourceFile(const char *pPath); //生成UnMarshaller.cpp
    TInt32 GenerateCSharpSkeleton(const char*pPath);
    TInt32 GenerateCSharpStub(const char*pPath);
};



}

#endif

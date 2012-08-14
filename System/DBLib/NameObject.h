#ifndef __DB_LIB_NAME_OBJECT_H__
#define __DB_LIB_NAME_OBJECT_H__



#include <string>
#include <map>

#define OBJECT_TYPE_ID_SEC	0xffff0000
#define OBJECT_TYPE_ID_ID	0x0000ffff

#define MAX_OBJECT_NAME		32

class CNameObject;

typedef CNameObject * (*OBJECT_CREATE_FN)();




enum OBJECT_CLONE_PARAM
{
	OBJECT_CLONE_WITHOUT_CHILD=1,
};


struct CLASS_INFO
{
	const char*				ClassName;
	CLASS_INFO *		pParentClassInfo;
	OBJECT_CREATE_FN	pObjectCreateFn;
	bool operator==(const CLASS_INFO& ClassInfo)
	{
		return strcmp(ClassName,ClassInfo.ClassName)==0;
	}
	bool operator!=(const CLASS_INFO& ClassInfo)
	{
		return strcmp(ClassName,ClassInfo.ClassName)!=0;
	}
	bool operator==(const char*  Name)
	{
		return strcmp(ClassName,Name)==0;
	}
	bool operator!=(const char* Name)
	{
		return strcmp(ClassName,Name)!=0;
	}
	bool IsKindOf(const CLASS_INFO& ClassInfo)
	{
		if(*this==ClassInfo)
			return true;
		if(pParentClassInfo)
			return pParentClassInfo->IsKindOf(ClassInfo);
		return false;
	}
	bool IsKindOf(const char* Name)
	{
		if(*this==Name)
			return true;
		if(pParentClassInfo)
			return pParentClassInfo->IsKindOf(Name);
		return false;
	}
	bool IsKindOfFast(const CLASS_INFO& ClassInfo)
	{
		if(this==&ClassInfo)
			return true;
		if(pParentClassInfo)
			return pParentClassInfo->IsKindOfFast(ClassInfo);
		return false;
	}
	CNameObject * CreateObject()
	{
		if(pObjectCreateFn)
			return (*pObjectCreateFn)();
		else
			return NULL;
	}
};


#define DECLARE_CLASS_INFO(ClassName) \
protected:\
	static CLASS_INFO	m_##ClassName##ClassInfo;\
public:\
	virtual CLASS_INFO& GetClassInfo()\
	{\
		return GetThisClassInfo();\
	}\
	static CNameObject * CreateObject();\
	static CLASS_INFO& GetThisClassInfo();\



#define GET_CLASS_INFO(ClassName) (ClassName::GetThisClassInfo())
#define GET_CLASS_INFO_BY_NAME(ClassName) (*ClassName::GetClassInfo(ClassName))

#define IMPLEMENT_CLASS_INFO(ClassName,ParentClassName) \
	CLASS_INFO	ClassName::m_##ClassName##ClassInfo={#ClassName,&GET_CLASS_INFO(ParentClassName),ClassName::CreateObject};\
	CNameObject * ClassName::CreateObject()\
	{\
		return (ParentClassName *) new ClassName();\
	}\
	CLASS_INFO& ClassName::GetThisClassInfo()\
	{\
		return m_##ClassName##ClassInfo;\
	}

#define DECLARE_CLASS_INFO_STATIC(ClassName) \
protected:\
	static CLASS_INFO	m_##ClassName##ClassInfo;\
public:\
	virtual CLASS_INFO& GetClassInfo()\
	{\
		return GetThisClassInfo();\
	}\
	static CLASS_INFO& GetThisClassInfo();\


#define IMPLEMENT_CLASS_INFO_STATIC(ClassName,ParentClassName) \
	CLASS_INFO	ClassName::m_##ClassName##ClassInfo={#ClassName,&GET_CLASS_INFO(ParentClassName),NULL};\
	CLASS_INFO& ClassName::GetThisClassInfo()\
	{\
		return m_##ClassName##ClassInfo;\
	}






class CUSOFile;


class CNameObject //: public CObject
{
protected:
	unsigned int			m_UpBoundGarder;
	unsigned int			m_ID;
	volatile unsigned int	m_UseRef;
	const void*			m_pParam;
	char			m_Name[MAX_OBJECT_NAME];

	static CLASS_INFO	m_CNameObjectClassInfo;
	
public:
	static CLASS_INFO& GetThisClassInfo();
	static CNameObject * CreateObject();	

	virtual CLASS_INFO& GetClassInfo()
	{
		return GetThisClassInfo();;
	}
	inline bool IsKindOf(const CLASS_INFO& ClassInfo)
	{
		return GetClassInfo().IsKindOf(ClassInfo);
	}
	inline bool IsKindOf(const char*  Name)
	{
		return GetClassInfo().IsKindOf(Name);
	}
	inline bool IsClass(const CLASS_INFO& ClassInfo)
	{
		return GetClassInfo()==ClassInfo;
	}
	inline bool IsClass(const char* Name)
	{
		return GetClassInfo()==Name;
	}
	inline bool IsKindOfFast(const CLASS_INFO& ClassInfo)
	{
		return GetClassInfo().IsKindOfFast(ClassInfo);
	}
	inline const char*GetClassName()
	{
		return GetClassInfo().ClassName;
	}

	CNameObject()
	{
		m_ID=0;
		m_UseRef=1;
		m_pParam=NULL;
		m_Name[0]=0;
	}

	virtual ~CNameObject()
	{

	}
	virtual bool Initialize()
	{
		return true;
	}
	virtual void Destory()
	{
	}
	void AddUseRef()
	{
		m_UseRef++;
	}
	void DecUseRef()
	{
		m_UseRef--;
	}
	unsigned int GetUseRef()
	{
		return m_UseRef;
	}
	virtual void Release()
	{
		DecUseRef();
		if(GetUseRef()<=0)
			delete this;
	}

	void SetName(const char* Name)
	{
		strncpy(m_Name,Name,MAX_OBJECT_NAME);
	}

	const char*GetName()
	{
		return m_Name;
	}

	void SetID(unsigned int ID)
	{
        m_ID=ID;
    }

	unsigned int GetID()
	{return m_ID;}


	void SetParam(const void* pParam)
	{
		m_pParam=pParam;
	}

	const void* GetParam()
	{
		return m_pParam;
	}	
	

};
#endif

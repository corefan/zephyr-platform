/*-------------------------------------------------------------
 Copyright (C)| 
 File: Doid.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  �ֲ�ʽ�����ַ
 Others: virtualIp = 0������Ϊ���Ľڵ㣬����ڵ��ͨ��\����������\���������.
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
 
#ifndef _S_DOID_H_
#define _S_DOID_H_


#include "TypeDef.h"
#include "SysMacros.h"

namespace Zephyr
{
//now we use this one, that is much more short.
class CDoid
{

public:

	TUInt16 m_objId;                   //obj idx;
    TUInt16 m_srvId;             //srv id, decide the port,ÿ������ip�Ͽ����ܶ��service.

    TUInt16 m_virtualIp;               //һ��������ڵ�����ip
    TUInt16 m_nodeId;           //һ�������Ϊ1���ڵ�
public:
	CDoid()
	{
 #ifdef _DEBUG
        m_nodeId = 0xFFFF; 
        m_virtualIp = 0xFFFF;
        m_srvId = 0xFFFF;
        m_objId = 0xFFFF;
 #endif
	}
    CDoid(TUInt16 nNodeId,TUInt16 nVip,TUInt16 nSrvId,TUInt16 nObjId)
    {
        m_nodeId = nNodeId;
        m_virtualIp = nVip;
        m_srvId = nSrvId;
        m_objId = nObjId;
    }
	
    ~CDoid()
    {

    }
    TBool IsInOneIp(CDoid &another)
    {
        if (m_nodeId != another.m_nodeId)
        {
            return FALSE;
        }
        if (m_virtualIp != another.m_virtualIp)
        {
            return FALSE;
        }
        return TRUE;
    }
    
	inline bool operator==(const CDoid& rvalue) const 
	{
        return (*((const TUInt64 *)this) == *((const TUInt64 *)(&rvalue)));
		//return ((m_objId == rvalue.m_objId)&&(m_srvId == rvalue.m_srvId)&&(m_virtualIp == rvalue.m_virtualIp)&&(m_nodeId == rvalue.m_nodeId));
	}
    inline bool operator>(const CDoid &rvalue) const
    {
        return (*((const TUInt64 *)this) > *((const TUInt64 *)(&rvalue))); 
    }
    inline bool operator < (const CDoid &rvalue) const
    {
        return (*((const TUInt64 *)this) < *((const TUInt64 *)(&rvalue)));  
    }
	inline TUInt64 ToTUIn64() const
	{
		return *((TUInt64 *)this);
	}

    inline CDoid &operator=(TUInt64& nNewValue)
	{
		*((TUInt64 *)this) = nNewValue;
        return *this;
	}
    //Buff need 32 bytes!
	void ToStr(TChar *pBuff);
};


const CDoid NULL_DOID;
}

#endif

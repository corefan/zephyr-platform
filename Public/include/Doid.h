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
    TUInt16 m_nodeId;           //һ�������Ϊ1���ڵ�
	TUInt16 m_virtualIp;               //һ��������ڵ�����ip
	TUInt16 m_srvId;             //srv id, decide the port,ÿ������ip�Ͽ����ܶ��service.
	TUInt16 m_objId;                   //obj idx;
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
	
    ~CDoid()
    {

    }

	bool operator==(const CDoid& doid)
	{
        //Temporary
		return ((m_objId == doid.m_objId)&&(m_srvId == doid.m_srvId)&&(m_virtualIp == doid.m_virtualIp)&&(m_nodeId == doid.m_nodeId));
	}

	TUInt64 ToTUIn64()
	{
		return *((TUInt64 *)this);
	}

    CDoid &operator=(TUInt64& nNewValue)
	{
		*((TUInt64 *)this) = nNewValue;
        return *this;
	}
    //Buff need 32 bytes!
	void ToStr(TUChar *pBuff);
};


const CDoid NULL_DOID;
}

#endif

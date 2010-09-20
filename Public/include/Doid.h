/*-------------------------------------------------------------
 Copyright (C)| 
 File: Doid.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  分布式对象地址
 Others: virtualIp = 0用来作为中心节点，负责节点间通信\命名服务器\管理服务器.
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
    TUInt16 m_nodeId;           //一组服务器为1个节点
	TUInt16 m_virtualIp;               //一组服务器内的虚拟ip
	TUInt16 m_srvId;             //srv id, decide the port,每个虚拟ip上可以跑多个service.
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

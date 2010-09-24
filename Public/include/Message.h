/*-------------------------------------------------------------
 Copyright (C)| 
 File: Message.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  对象间消息
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
#ifndef S_MESSAGE_H
#define S_MESSAGE_H


#include "TypeDef.h"


#include "Doid.h"
#include "SysMacros.h"

namespace Zephyr
{

enum EnConnectEvent
{
    en_connection_is_broken_event             = 0,
    en_connection_is_established_event        = 1,
    en_connection_is_reestablished_event      = 2,
    en_connection_can_not_established_event   = 3,
};

struct CConnectionEvent
{
    unsigned int m_vip;
    unsigned int m_event;
    void         *m_pData;
};



//the following was used for srv

enum EnByteOrder
{
    little_endian         = 0,
    big_endian            = 1,
};

class CMessageHeader
{
private:
    //the 1st  bit indicate the byte-orders of this msg.
    //the following 7 bits indicate how many dest doid does this header has;
    //the rest 24 bits stored the length of this msg body
    //TUInt64 m_msgInfo;
    union
    {
        struct 
        {
            TUInt32    m_msgLength:24;
            TUInt32    m_nrOfDestDoid:7;
            TUInt32    m_byteOrder:1;
            
            //最多16M,放在前面，在小端机上，就不用位移了.
            
            //服务8个bit, 接口6,方法 12比特
            TUInt32    m_methodId:24;
            //the 2 bits indicated transport priority, (default yes,that means we will retry the sending.)
            TUInt32    m_priority:2;
            //the 2nd bit indicate whether if we should report the exceptions (Default yes)
            TUInt32    m_reportExcep:1;
            //the 3rd bits indicated if it is marked to be traced. Reserved.
            TUInt32    m_needTrace:1;
            //the 4th bits indicated if it needs synchronization,that means the skeleton will hang up.  Reserved.
            TUInt32    m_needSyn:1;
            // no use.
            TUInt32    m_reserved:1;
            //the 5th bits indicated if it is a system call and it should not be reported to the application.
            //1 for net layer,heartbeat, 2 for system calls,like for time synchronization. service heartbeat. service managerment and so on.
            TUInt32    m_systemCall:1;
            TUInt32    m_reply:1;
        };
        TUInt64 m_data;
    } m_msgInfo;

    CDoid m_srcDoid;           //the source obj id
    CDoid m_destDoid;          //the destination obj id(may have a lot, max 256)
    TUInt16 m_timeStamp;
    TUChar m_checkSum[2];



public:
   
    inline TUChar *GetBuffer()
    {
        return (TUChar*)((TUChar*)this + sizeof (CMessageHeader));
    }

    inline TUInt32 GetLength()
    {
        return m_msgInfo.m_msgLength;
    }

    inline void SetPriority(TUInt32 prioity)
    {
        m_msgInfo.m_priority = prioity;
    }
    inline TUInt32 GetPriority()
    {
        return m_msgInfo.m_priority;
    }

	inline TUInt32 GetDestDoidNum()
    {
        return m_msgInfo.m_nrOfDestDoid;
    }
    inline TInt32 SetDestDoidNum(TUInt32 num)
    {
        if (num > 127)
        {
            return OUT_OF_RANGE;
        }
        m_msgInfo.m_nrOfDestDoid = num;
    }
    inline CDoid  *GetDestDoidByIdx(TUInt32 idx = 0)
    {
        if (0 == idx)
        {
            return &m_destDoid;
        }
        else
        {
            //不检查了
            //if (idx < m_msgInfo.m_nrOfDestDoid)
            {
                return (CDoid  *) ((TUChar*)this + sizeof(CMessageHeader) + GetBodyLength() + sizeof(CDoid) * (idx - 1));
            }
        }
    }
    inline CDoid *GetMultiDestDoids()
    {
        if (m_msgInfo.m_nrOfDestDoid > 1)
        {
            return (CDoid  *) ((char*)this + sizeof(CMessageHeader) + GetBodyLength());
        }
        return NULL;
    }
    //get the byte order of the following data.
    inline TUInt32 GetByteOrders()
    {
        return m_msgInfo.m_byteOrder;
    }

    inline TUInt32 GetBodyLength()
    {
        return (m_msgInfo.m_msgLength
                - sizeof(CMessageHeader)
                - sizeof(CDoid) * (m_msgInfo.m_nrOfDestDoid - 1));
    }
    //client only need know these things above.

    inline CDoid *GetSrcDoid()
    {
        return &m_srcDoid;
    }

    inline TUInt32 GetMethodId()
    {
        return (m_msgInfo.m_methodId);
    }

	inline void SetTimeStamp(TUInt16 timeStamp)
	{
		m_timeStamp = timeStamp;
	}
	inline TUInt16 GetTimeStamp()
	{
		return m_timeStamp;
	}
    inline void PrepareToSend(TUChar checkCode1,TUChar checkCode2)
    {
        TUChar* pBuffer = (TUChar*)this;
		m_checkSum[0] = checkCode1;
		m_checkSum[1] = checkCode2;
		int len = (sizeof(CMessageHeader)-2);
        for(int i =0;i<len;i+=2)
        {
            m_checkSum[0] = m_checkSum[0] ^ pBuffer[i];
            m_checkSum[1] = m_checkSum[1] ^ pBuffer[i+1];
        }
    }
    
    TInt32 Init(TUInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* pDestDoids,TUInt32 destDoidNum);

    void Compack4Client(TUChar *pBuff)
    {
        
    }
    TInt32 GetCompackedLen()
    {
        //6bytes for compacked header
        return GetBodyLength() + 6;
    }
    //使用
    inline TUInt32 RemarkHead(CDoid *pDoid,TUChar checkCode1,TUChar checkCode2)
    {
        m_destDoid = *pDoid;
        m_msgInfo.m_nrOfDestDoid = 1;
        PrepareToSend(checkCode1,checkCode2);
    }
    
//private:
    //can be called by the communicator only. these are internal info;
    //no use now
    /*bool IsOnExtension()
    {
        return false;
    }
    */
    //no use now
    //int IsSliced();

private:
	//to make sure you could only create the message by msgPool.
	CMessageHeader()
	{
	    m_msgInfo.m_data = 0;
	};
    //set to limit the "=" operation.
	CMessageHeader &operator=(CMessageHeader& msg){};

};



//make sure len > sizeof(SCTDMessageHeader)
}
#endif


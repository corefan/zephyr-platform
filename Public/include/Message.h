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
    TUInt16      m_vip;
    TUInt16      m_event;
    void         *m_pData;
};



//the following was used for srv

enum EnByteOrder
{
    little_endian         = 0x04000000,
    big_endian            = 0x00000000,
};

enum EnTransportPriority
{
    send_priority_mask    = 0xC0000000,
	send_immediately      = 0x00000000,
	send_accumulately     = 0x40000000,
	send_every_twice      = 0x80000000,
	//add more maybe
};

class CMessageHeader
{
private:
    //the first 1 bit indicate if this msg is actived,
    //the 2nd  bit indicate the byte-orders of this msg.
    //the following 8 bits indicate how many dest doid does this header has;
    //the rest 22 bits stored the length of this msg body
    TUInt32 m_msgInfo;

	//the first 2 bits indicated transport priority.
	//
	//the 3rd bits indicated if it is marked to be traced. Reserved.
	//the 4th bits indicated if it needs synchronization.  Reserved.
	TUInt32  m_methodId;             //method Id
    CDoid m_srcDoid;           //the source obj id
    CDoid m_destDoid;          //the destination obj id(may have a lot, max 256)
    TUInt16 m_timeStamp;
    TUChar m_checkSum[2];



public:
    enum EnIsActived
    {
        is_acitved_mask       = 0x80000000,
    };

    enum EnByteOrder
    {
        byte_order_mask       = 0x40000000,
    };

    enum EnNrOfDestDoid
	{
	    num_of_dest_doid_mask = 0x3FC00000,
	};

    enum EnMessageLength
	{
	    message_length_mask   = 0x003FFFFF,

	};


	enum EnMethodID
	{
	    method_id_mask        = 0x3FFFFFFF,
	};

public:
    //initialize the message header

    //get the buffer to write or read .
    inline TUChar *GetBuffer()
    {
        return (TUChar*)((TUChar*)this + sizeof (CMessageHeader));
    }

    inline TUInt32 GetLength()
    {
        return (m_msgInfo & message_length_mask);
    }

    inline void SetPriority(EnTransportPriority pri)
    {
        //BEGIN MODIDY s0032 03-16-2009
        //m_methodId = m_methodId & 0x0FFFFFFF;

        m_methodId = m_methodId & method_id_mask;
        //END MODIDY s0032 03-16-2009
        m_methodId = m_methodId | pri;
    }
    inline EnTransportPriority GetPriority()
    {
        return static_cast<EnTransportPriority>(m_methodId & send_priority_mask);
    }

	inline TUInt32 GetDestDoidNum()
    {
        TUInt32 num = m_msgInfo & num_of_dest_doid_mask;
        return (num>>22);
    }
    inline TInt32 SetDestDoidNum(TUInt32 num)
    {
        if (num <=0)
        {
            return FAIL;
        }
        num = num<<22;
        num = (num & num_of_dest_doid_mask);
        m_msgInfo = m_msgInfo & 0xC03FFFFF;
        m_msgInfo = m_msgInfo | num;
        return SUCCESS;
    }
    inline CDoid  *GetDestDoidByIdx(TUInt32 idx = 0)
    {
        if (0 == idx)
        {
            return &m_destDoid;
        }
        else
        {
            return (CDoid  *) ((TUChar*)this + sizeof(CMessageHeader) + GetBodyLength() + sizeof(CDoid) * (idx - 1));
        }
    }
    inline CDoid *GetMultiDestDoids()
    {
        if (GetDestDoidNum() > 1)
        {
            return (CDoid  *) ((char*)this + sizeof(CMessageHeader) + GetBodyLength());
        }
        return NULL;
    }
    //get the byte order of the following data.
    inline EnByteOrder GetByteOrders()
    {
        return static_cast<EnByteOrder>(m_msgInfo & byte_order_mask);
    }

    inline TUInt32 GetBodyLength()
    {
        return ((m_msgInfo & message_length_mask)
                - sizeof(CMessageHeader)
                - sizeof(CDoid) * (GetDestDoidNum() - 1));
    }
    //client only need know these things above.

    inline CDoid *GetSrcDoid()
    {
        return &m_srcDoid;
    }

    inline TUInt32 GetMethodId()
    {
        return (m_methodId & method_id_mask);
    }

	inline void SetTimeStamp(TUInt16 timeStamp)
	{
		m_timeStamp = timeStamp;
	}
	inline TUInt16 GetTimeStamp()
	{
		return m_timeStamp;
	}
    inline void PrepareToSend()
    {
        TUChar* pBuffer = (TUChar*)this;
		m_checkSum[0] = 'S';
		m_checkSum[1] = 'Z';
        for(int i =0;i<(sizeof(CMessageHeader)-2);i+=2)
        {
            m_checkSum[0] = m_checkSum[0] ^ pBuffer[i];
            m_checkSum[1] = m_checkSum[1] ^ pBuffer[i+1];
        }
    }
    TInt32 Init(TUInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* pDestDoids,TUInt32 destDoidNum);


    inline TUInt32 RemarkHead(TUInt32 destDoidIdx)
    {
        
        TUInt32 destDoidNr = GetDestDoidNum();
        if (destDoidNr == 1)
        {
            return m_msgInfo;
        }
        TUInt32 origInfo  = m_msgInfo;
        if (destDoidIdx > destDoidNr)
        {
            return m_msgInfo;
        }


        TUInt32 len = GetLength();
        TUInt32 sendLen = len - sizeof(CDoid) * (destDoidNr - 1);
        
        SetDestDoidNum(1);

        m_msgInfo &= ~(CMessageHeader::message_length_mask);
        m_msgInfo |= (unsigned int)sendLen;
        if (destDoidIdx)
        {
            m_destDoid = *(GetDestDoidByIdx(destDoidIdx));
        }
                            
        return origInfo;
    }
    
    void RetreatHead(TUInt32 origInfo)
    {
        m_msgInfo = origInfo;
    }
    
	TInt32 SetBodyLength(TUInt32 bodyLength);
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



    //Core use only. may be it's not necessary.
    inline void Active()
    {
        m_msgInfo = m_msgInfo | is_acitved_mask;
        //than turn m_msgInfo into net byte order
        /*
                 int netOrderInfo = htonl(m_msgInfo);
                if (netOrderInfo == m_msgInfo)
                {
                    m_msgInfo = m_msgInfo | EnByteOrder;
                    netOrderInfo = htonl(m_msgInfo);
                }
                m_msgInfo = netOrderInfo;
        */
    }

    inline void PreProcOnNetMsg()
    {
        //m_msgInfo = htonl(m_msgInfo);
    }

    inline void Deactive()
    {
        m_msgInfo = m_msgInfo & 0x7FFFFFFF;
    }
    TBool IsActived()
    {
        if (((m_msgInfo & is_acitved_mask)))
        {
            return TRUE;
        }
        return FALSE;
    }
private:
	//to make sure you could only create the message by msgPool.
	CMessageHeader()
	{
	    m_msgInfo = 0;
        m_methodId = 0;

	};
    //set to limit the "=" operation.
	CMessageHeader &operator=(CMessageHeader& msg){};

};


//make sure len > sizeof(SCTDMessageHeader)
}
#endif


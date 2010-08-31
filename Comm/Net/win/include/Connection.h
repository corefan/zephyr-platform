/*-------------------------------------------------------------
 Copyright (C)| 
 File: Connection.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 连接层，标示一个连接
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/



 #ifndef __ZEPHYR_CONNECTION_H__
 #define __ZEPHYR_CONNECTION_H__

#include "TypeDef.h"

#include "SysMacros.h"

#include "Pipe.h"

#include "IocpOverlappedDataHeader.h"

#include "IfNetData.h"
#include "Lock.h"
#include "IfNetApp.h"
namespace Zephyr
{


enum EnConnectionState
{
    connection_is_not_in_use            = 0x00,
    connection_is_trying                = 0x01,
    connection_is_established           = 0x02,
    connection_is_using                 = 0x03,
    connection_is_broken                = 0x04,
    connection_is_aborted               = 0x05,
};

enum EnConncectionType
{
    connection_is_passive               = 0,
    connection_is_postive               = 1,
};

enum EnConnectionEvent
{
    event_connection_nothing            = 0,
    event_connection_broken             = 1,
    event_connection_has_new_data       = 2,
};
struct TIOEvent
{
     TInt32                     m_connectionIdx;
     TInt32                     m_seqNum;
};

class CConnectionMgr;

class CConnection
{
private:
    //Record the file Handle.
    CConPair                    m_key;
    SOCKET                      m_socket;
    TInt16                      m_connectionIdx;
    TInt16	                    m_errorCode;
    TUInt32                     m_seqNum;
    TInt16                      m_connectionState;
    TInt16                      m_connectionType;
    void                        *m_pAppData;
    CPipe                       m_inPipe;
    CPipe                       m_outPipe;
    TUChar                      *m_pConfirmedTo;
    CLock                                       m_wLock;
    CLock                                       m_rLock;
    #ifdef _DEBUG
    TUInt64                     m_msgRecv;
    TUInt64                     m_msgSend;
    #endif
    //TUChar m_pRecvBuff[1024*4];
    CIocpOverlappedDataHeader   m_readDataHeader;
    CIocpOverlappedDataHeader   m_writeDataHeader;
    CConnection                 *m_pNext;
    CConnection                 *m_pPrev;
    
    volatile TUInt32            m_appSeqNum;
    volatile TUInt32            m_netConfirmNum;
    
    volatile TUInt32            m_netSeqNum;
    volatile TUInt32            m_appConfirmNum;
    
    CConnectionMgr              *m_pMgr;
public:
    
    void Callback(IfNetApp *pApp);
    TUInt32 GetEvent()
    {
        if (m_connectionState == connection_is_not_in_use)
        {
            return event_connection_nothing;
        }
        TUChar *pNewTo = m_inPipe.GetDataHeader();
        if (pNewTo != m_pConfirmedTo)
        {
            //m_pConfirmedTo = pNewTo;
            return event_connection_has_new_data;
        }
        if (m_connectionState == connection_is_broken)
        {
            return event_connection_broken;
        }
        return event_connection_nothing;
    }
    TBool IsConnected()
    {
        if (connection_is_using == m_connectionState)
        {
            return TRUE;
        }
        return FALSE;
    }
    CConnection *GetNext()
    {
        return m_pNext;
    }
    void OnConnected()
    {
        m_connectionState = connection_is_established;
        m_connectionType = connection_is_postive;
    }
    
    TInt32 GetConnectionState()
    {
        return m_connectionState;
    }
    TInt32 GetConnectionType()
    {
        return m_connectionType;
    }
    void *GetAppData()
    {
        return m_pAppData;
    }
    TUInt16 GetConnectionIdx()
    {
        return m_connectionIdx;
    }
    TUInt32 GetSeqNum()
    {
        return m_seqNum;
    }
    CConPair *GetConPair()
    {
        return &m_key;
    }
    void SetSocket(SOCKET socket)
    {
        m_socket = socket;
    }
    TInt32 SetSocketOnAccept(SOCKET socket,CConnectionMgr *pMgr);
    SOCKET GetSocket()
    {
        return m_socket;
    }
    TInt32 OnCreate(TUInt32 idx,TUInt32 pipeSize);
    TBool CheckReadHeader(CIocpOverlappedDataHeader *pHeader)
    {
        if (pHeader == &m_readDataHeader)
        {
            if (m_readDataHeader.m_seqNum == m_seqNum)
            {
                return TRUE;
            }
        }
        return FALSE;
    }
    void SetAppData(void *pData)
    {
        m_pAppData = pData;
    }
    TBool CheckWriteHeader(CIocpOverlappedDataHeader *pHeader)
    {
        if (pHeader == &m_writeDataHeader)
        {
            if (m_writeDataHeader.m_seqNum == m_seqNum)
            {
                return TRUE;
            }
        }
        return FALSE;
    }
    TInt32 SetSocketOptions();
    TInt32 OnInit();
    TInt32 OnFinal();
    TInt32 Init(TUInt32 remoteIp,TUInt32 myIp, TUInt16 remotePort,TUInt16 myPort,TUInt32 connectionType,void *pAppData,CConnectionMgr *pMgr);
    TInt32 AppSend(TUChar *pData,TUInt32 dataLen);
    TInt32 GetAvailableReadLength()
    {
        return m_inPipe.GetDataLen();
    }
    //TUChar *Peek(TUChar *pData,TUInt32 needLength);
    
    TInt32 OnRecv(CIocpOverlappedDataHeader *pHeader,TUInt32 ioSize);
    TInt32 OnSent(CIocpOverlappedDataHeader *m_writeDataHeader,TUInt32 ioSize);
    CIocpOverlappedDataHeader *GetWritingData();
	CIocpOverlappedDataHeader *PrepareToRead();
    //disconnect the connection
	void Disconnect();
    void ConnectionIsTrying();
    void ConnectionIsEstablished()
    {
        m_connectionState = connection_is_established;
        m_connectionType  = connection_is_passive;
    }
    void ConnectionIsBroken();
    TInt32 Routine();
    void AttachToHead(CConnection *pConnection)
    {
        m_pPrev = NULL;
        if (pConnection)
        {
            pConnection->m_pPrev = this;
        }
        m_pNext = pConnection;
    }
    void Detach()
    {
        if (m_pPrev)
        {
            m_pPrev->m_pNext = m_pNext;
        }
        if (m_pNext)
        {
            m_pNext->m_pPrev = m_pPrev;
        }
        m_pNext = NULL;
        m_pPrev = NULL;
    }
    void SetConnectionState(EnConnectionState state)
    {
        m_connectionState = state;
    }
    
  
    TInt32 AddNewData(TChar *pBuff);
private:
    ;
};

}

 #endif
 

/*-------------------------------------------------------------
 Copyright (C)| 
 File: NetTask.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  网络层工人，每人维护一个completionPort,维护一个connection列表然后不停的收发数据
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
#ifndef __ZEPHYR_NET_TASK_H__
#define __ZEPHYR_NET_TASK_H__
//#include "TplMap.h"
#include "IfTask.h"
#include "Connection.h"

namespace Zephyr
{





struct CNetEvent
{
    TUInt16      m_key;
    TUInt16      m_seqNum;
    TUInt16      m_isUsed;
    TUInt16      m_eventType;
};


class CNetTask : public IfTask
{
protected:
    void*  m_completionPort;
    //TplMap<CConnection,TUInt32>                 m_connectionPool;
    //TplNode<CNetEvent,TUInt16>           *m_pWorkerEventPool[NR_OF_QUEUE_IN_TASK];

    //events that net task generated.
    //TplNode<CNetEvent,TUInt16> *m_pNetTaskEvent[NR_OF_QUEUE_IN_TASK];
    ////events that app generated.
    //TplNode<CNetEvent,TUInt16> *m_pAppEvent[NR_OF_QUEUE_IN_TASK];
    
    
    TUInt32                                     m_maxConnectionNr;
    CConnection                                 *m_pConnections;

    
    volatile  TUInt32                           m_connectionAborted;
#ifdef _DEBUG
    TUInt32                                     m_startTime;
    TUInt32                                     m_runCnt;
    TUInt32                                     m_averageTime;
#endif

public:
    TInt32         Init(TInt32 maxNrOfConnection,CConnection *pConnections,void *completionPort);

    void* GetCompletionPort()
    {
        return m_completionPort;
    }
    TInt32 GetAbortedConnectionNr()
    {
        return (m_connectionAborted);
    }

    virtual TInt32 Begin(TInt32 threadId);
    virtual TInt32 Run(const TInt32 threadId,const TInt32 runCnt);
    virtual TInt32 End(TInt32 threadId);
private:
    
    //void   CloseConnection(CConnection *pConnection);

    //put the connection in to app
    //TInt32 OnAppSend(TplNode<CNetEvent,TUInt16>* pConnection);

    TInt32 OnDisconnected(CConnection *pConnection);
    
    //inline TInt32 AddNetEvent(CConnection *pConnection,TUInt16 eventType);
    TInt32 ProcessIOMessage(CIocpOverlappedDataHeader *pHeader,TUInt32 ioSize);
    //TInt32 Send(CIocpOverlappedDataHeader *pHeader);
    //TInt32 Recv(CIocpOverlappedDataHeader *pHeader);
    
};


}



#endif

 

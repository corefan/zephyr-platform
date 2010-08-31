/*-------------------------------------------------------------
 Copyright (C)| 
 File: ConnectionMgr.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 连接层，标示一个连接管理者
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/



#ifndef __ZEPHYR_CONNECTION_MGR_H__
#define __ZEPHYR_CONNECTION_MGR_H__

#include "TypeDef.h"
#include "SysMacros.h"
#include "connection.h"
#include "IfNet.h"
#include "IfNetApp.h"
#include "IfTask.h"
#include "IfTaskMgr.h"
#include "NetTask.h"
#include "Listener.h"
#include "IfParser.h"
#include "Connector.h"

namespace Zephyr
{
    
class CConnectionMgr : public IfNet
{
protected:
    CListener       *m_pListeners;
    TUInt16         m_nrOfListener;
    TUInt16         m_maxConnectionNum;
    CConnection     *m_pConnectionPool;
    CConnection     *m_pFree; 
    CConnection     *m_pUsed;
    CConnector      m_connector;
    CNetTask        m_netWorkers[NR_OF_NET_WORKER];
    IfNetApp        *m_pApp;
    IfTaskMgr       *m_pTaskMgr;
    HANDLE          m_hCompletionPort;
    TUInt32         m_lastRunTo;
    //网络层事件
    CPipe           m_netEvents;
    //应用层事件
    CPipe           m_appEvents;
private:
    
    
public:
    CConnectionMgr(IfNetApp *pApp,IfTaskMgr *pTaskMgr);
    ~CConnectionMgr();

    TInt32 Init(TUInt32 maxConnectionNum,TUInt32 buffSize = (256*1024));
    virtual TInt32 SendMsg(TInt32 toConnectionIdx,TUChar *pMsg,TUInt32 msgLen);
    //if connect return SUCCESS, just means the operation is going on, but not really success.
    //If it is connected or failed, it will infoed later in IfNetApp::OnConnected or IfNetApp::OnNetError.
    virtual TInt32 Connect(TChar *pRemoteIp,TChar *pMyIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData);
    virtual TInt32 Connect(TUInt32 remoteIp,TUInt32 myIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData);
    virtual TInt32 Run(TUInt32 runCnt);
    virtual TInt32 Disconnect(TUInt32 connectionIdx);
    virtual TInt32 SetAppData(TUInt32 connectionIdx,void *pAppData);
    virtual TInt32 Listen(TUInt32 ip,TUInt16 port,TUInt16 maxConnection,void *pAppData);
    virtual CConPair *GetConnectionInfo(TUInt32 connectionIdx);
    CConnection *GetConnection();
    void ReleaseConnection(CConnection *pConnection);
    
    TInt32 AddNetEvent(TIOEvent& event);
    
    TInt32 AddAppEvent(TIOEvent& event);
    
    TIOEvent *GetNetEvent();
    
    TIOEvent *GetAppEvent();
    
};



}

#endif
 
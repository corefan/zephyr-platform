/*-------------------------------------------------------------
 Copyright (C)| 
 File: ConnectionMgr.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: ���Ӳ㣬��ʾһ�����ӹ�����
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
#include "NetEventQueue.h"
#include <stdio.h>
namespace Zephyr
{


class CConnectionMgr : public IfNet
{
protected:
    CListener*          m_pListeners;
    TUInt16             m_nrOfListener;

    HANDLE              m_hCompletionPort;
    
    CConnector          m_connector;
    //һ��worker
    CNetTask            m_netWorker;
    CConnectionPool     m_conncectionPool;
    
    IfParserFactory*    m_pParserFactory;
    IfCryptorFactory*   m_pCryptorFactory;
    
    CNetEventQueues     m_netEventQueues;
//         
//         //TUInt32           m_lastRunTo;
//         //������¼�
//         CIoEventQueue       m_netEvents;
//         //Ӧ�ò��¼�
//         CIoEventQueue       m_appEvents;
private:
    TUInt32                                     m_buffSize;
    TUChar*                                     m_pBuff;

public:
    //��������
    virtual TInt32 Connect(TChar *pRemoteIp,TChar *pMyIp,TUInt16 remotePort,TUInt16 myPort,void *pAppCallBack);
    //virtual TInt32 Connect(TUInt32 remoteIp,TUInt32 myIp,TUInt16 remotePort,TUInt16 myPort,void *pAppCallBack) = 0;


    virtual TInt32 Listen(TChar *pIp,TUInt16 port,TUInt16 maxConnection,void *pIfCallBack);

    virtual TInt32 StopListening(TInt32 listeningIdx);

    //����Ĭ��parserFactory,���û�У����յ��κ���Ϣ�����ܳ��ȣ������ϱ�.
    virtual TInt32 SetDefaultParserFactory(void *pIfDefualtParserFactory);
    virtual TInt32 SetDefaultCryptorFactory(void *pIfCryptorFactory);
    //if chOpt = 1,disable the Nagle algrithom.
    //else enable it , it is enableed by default.
    //In general this is unnecessary.
    //����connection�ص�.
    virtual TInt32 Run(TUInt32 runCnt);
    
public:
    CConnectionMgr();
    ~CConnectionMgr();

    TInt32 Init(TUInt32 maxConnectionNum,IfTaskMgr *pTaskMgr,IfParserFactory* pParserFactory,IfCryptorFactory *pIfCryptorfactory = NULL,TUInt32 buffSize = 256*1024);
    void   Final();
    
};



}

#endif
 
#ifndef __ZEPHYR_COMM_MGR_H__
#define __ZEPHYR_COMM_MGR_H__
#include "TypeDef.h"
#include "IfCommunicator.h"
#include "Communicator.h"
#include "CommMacrosAndConsts.h"
#include "IfNet.h"
#include "IpMaps.h"
#include "TplPool.h"
#include "..\Public\Interface\Platform\include\IfTaskMgr.h"
#include "CommConnection.h"
#include "MsgParserFactory.h"
#include "TimeSystem.h"
#include "IfLoggerMgr.h"
namespace Zephyr
{

enum EnCommOpr
{
    en_comm_id              = 0,
    en_comm_read_net_msg,
    en_comm_send_net_msg,
    en_comm_read_net_msg_blocked,
    en_comm_send_net_msg_blocked,
};

class CCommMgr : public IfCommunicatorMgr, public IfTask
{
private:
    TUInt32             m_nrOfComm;
    CCommunicator       *m_pCommunicators;
    CIpMap              m_ipMaps;
    TUInt32             m_lastCheckTime;
    IfNet               *m_pNet;
    ItemClassPool<CCommConnection> m_connectionPool;
    CMsgParserFactory   *m_pParserFactory;
    CCommConnection     **m_ppConnections;

    EnCommOpr           m_enLastOpr;
    TUInt16             m_nNetBlockedOnIp;
    TUInt16             m_nAppBlockedOnSrv;
    TUInt32             m_netBlockTime;
    TUInt32             m_nAppBlockTime;

    CTimeSystem         m_timeSystem;
    TUChar              *m_pBuff;
    IfLoggerManager     *m_pLoggerMgr;
    IfLogger            *m_pLogger;

public:
    //taskMgr由ServerContainer生成.
    TInt32 Init(IfTaskMgr *pTaskMgr,IfLoggerManager *pIfLogMgr,const TChar *pConfigName=szDefaultCommConfigName);
    virtual IfCommunicator *RegisterWorker(TUInt16 srvId);

    virtual TInt32 Begin(TInt32 threadId)
    {
        return SUCCESS;
    }
    virtual TInt32 Run(const TInt32 threadId,const TInt32 runCnt);
    virtual TInt32 End(TInt32 threadId)
    {
        return SUCCESS;
    }
private:
    //返回值为是否需要丢弃消息,网络层阻塞了

    //返回值为是否需要丢弃消息,应用层阻塞了


    TBool IsAppStillBlocked();
    TBool IsNetStillBlocked();

    TBool CheckNetState(CMessageHeader *pMsg);
    void SendAppMsg(CMessageHeader *pMsg);

    //TInt32 DistributeSrvMsg(TInt32 idx);
};

}
#endif

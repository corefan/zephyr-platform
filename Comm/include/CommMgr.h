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
#include "include\CommConnection.h"
#include "include\MsgParserFactory.h"
namespace Zephyr
{


class CCommMgr : public IfCommunicatorMgr
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
public:
    //taskMgr”…ServerContainer…˙≥….
    TInt32 Init(IfTaskMgr *pTaskMgr,const TChar *pConfigName=szDefaultLoggerName);
    virtual IfCommunicator *RegisterWorker(TUInt16 srvId); 
};

}
#endif

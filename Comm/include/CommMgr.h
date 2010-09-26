#ifndef __ZEPHYR_COMM_MGR_H__
#define __ZEPHYR_COMM_MGR_H__
#include "TypeDef.h"
#include "IfCommunicator.h"
#include "Communicator.h"
#include "CommMacrosAndConsts.h"
#include "IfNet.h"
#include "IpMaps.h"
#include "TplPool.h"
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
public:
    TInt32 Init(const TChar *pConfigName=szDefaultLoggerName);
    virtual IfCommunicator *RegisterWorker(TUInt16 srvId); 
};

}
#endif

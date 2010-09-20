#ifndef __ZEPHYR_COMM_MGR_H__
#define __ZEPHYR_COMM_MGR_H__
#include "TypeDef.h"
#include "IfCommunicator.h"
#include "Communicator.h"
#include "CommMacrosAndConsts.h"

namespace Zephyr
{


class CCommMgr : public IfCommunicatorMgr
{
private:
    TUInt32       m_nrOfComm;
    CCommunicator *m_pCommunicators;
    CIpMap        m_ipMaps;
    TUInt32       m_lastCheckTime;
    IfNet         *m_pNet;
public:
    TInt32 Init(TChar *pConfigName=szDefaultLoggerName);
    virtual IfCommunicator* GetCommunicator(TInt32 srvId);
};

}
#endif

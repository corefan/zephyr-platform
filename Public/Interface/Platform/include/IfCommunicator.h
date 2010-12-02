#ifndef __ZEPHYR_PUBLIC_PLATFORM_IF_COMMUNICATOR_H__
#define __ZEPHYR_PUBLIC_PLATFORM_IF_COMMUNICATOR_H__

#include "Message.h"
#include "TypeDef.h"

namespace Zephyr
{

class IfCommunicator
{
public:

    //
    virtual CMessageHeader *GetMsg(TUInt32 needRetMsgBuff = 1) = 0;

    virtual void ReturnMsgBuff(CMessageHeader *pMsg) = 0;

    virtual CMessageHeader *PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* destDoid,TInt32 destDoidNum) = 0;

    //virtual int SendMsg(SCTDMessageHeader *,bool bNeedCopy = false) = 0;
    //do not need the para bNeedCopy any more, I will check it!
    virtual TInt32 SendMsg(CMessageHeader *pMsg) = 0;

    //application should not call this !!! called by work thread only! or else some events would lost!
    virtual CConnectionEvent GetConnectionEvent(TInt32& result) = 0;

    virtual TUInt32 GetTime() = 0;
    //maybe I could expose less header files
};

class IfCommunicatorMgr
{
public:
    virtual IfCommunicator *RegisterWorker(TUInt16 srvId) = 0;
};

}


#endif

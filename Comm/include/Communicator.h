#ifndef __ZEPHYR_COMM_COMMUNICATOR_H__
#define __ZEPHYR_COMM_COMMUNICATOR_H__

#include "TypeDef.h"
#include "IfCommunicator.h"
#include "Pipe.h"

namespace Zephyr
{

class CCommunicator : public IfCommunicator
{
protected:
    CPipe   m_inPipe;
    CPipe   m_outPipe;
    TUChar* m_pBuff;
    TUInt32 m_buffSize;
    CPipe   m_eventPool;
public:
    CCommunicator();
    ~CCommunicator();
    TInt32 Init(TUInt32 inPipeSize,TUInt32 outPipeSize,TUInt32 maxMessageSize);
    TInt32 InitEventPool(TUInt32 maxEventNr);
public:

    //add a new idx , that indicate the pipe to use!
    virtual CMessageHeader *GetMsg(TUInt32 needRetMsgBuff = 1);

    virtual void ReturnMsgBuff(CMessageHeader *pMsg);

    virtual CMessageHeader *PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* pDestDoid,TInt32 destDoidNum);

    //virtual int SendMsg(SCTDMessageHeader *,bool bNeedCopy = false) = 0;
    //do not need the para bNeedCopy any more, I will check it!`
    virtual TInt32 SendMsg(CMessageHeader *pMsg);

    //application should not call this !!! called by work thread only! or else some events would lost!
    virtual CConnectionEvent GetConnectionEvent(TInt32& result);
};

}

#endif

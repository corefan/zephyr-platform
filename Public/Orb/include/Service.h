#include "./DistributedObject.h"
#include "../../include/TypeDef.h"
#include "../../include/SysMacros.h"
#include "../../include/Timer.h"
#include "../../include/Clock.h"
#include "./Session.h"
#include "../../Interface/Platform/include/IfOrb.h"
namespace Zephyr
{

class CService : public CObject
{
protected:
    CTimer m_tTimer;
    const CClock  *m_pClock;
    IfOrb   *m_pIfOrb; //只有service才能注销
    IfCommunicator *m_pIfComm;
   
    TUInt32 m_nServiceId;

    void *m_pLock;
public:
    TUInt32 GetServiceId()
    {
        return m_nServiceId;
    }
    CService();
    void LockInterService();
    void UnlockInterService();
    virtual ~CService();
    TInt32 Init(const CClock *pClock,IfCommunicator *pIfComm,IfOrb *pOrb,CDoid *pDoid);
    TInt32 InitTimer(TInt32 nTimerNr)
    {
        return m_tTimer.Init(nTimerNr);
    }

    virtual TInt32  OnRoutine(TUInt32 nRunCnt);

    void UnRegisterSession(CSession *pSession);

    void *SetTimer(TUInt32 uGapInMs,TInt32 nRepeatTime,IfScheduler *pScheduler)
    {
        return m_tTimer.SetTimer(uGapInMs,nRepeatTime,pScheduler,m_pClock->GetPlatformTime());
    }
    //删除定时器pTimer是SetTimer返回的结果
    const IfScheduler *KillTimer(void *pTimer)
    {
        return m_tTimer.KillTimer(pTimer);
    }

    virtual const CClock *GetClock()
    {
        return m_pClock;
    }
    virtual IfTimer *GetTimer()
    {
        return &m_tTimer;
    }
    TInt32 RegisterSession(CSession *pSession);

};

}
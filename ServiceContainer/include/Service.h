#include "./DistributedObject.h"
#include "../../Public/include/TypeDef.h"
#include "../../Public/include/SysMacros.h"
#include "../../Public/include/Timer.h"
#include "../../Public/include/Clock.h"
#include "./Session.h"
#include "../../Public/Interface/Platform/include/IfOrb.h"
namespace Zephyr
{

class CService : public CObject
{
private:
    CTimer m_tTimer;
    const CClock  *m_pClock;
    IfOrb   *m_pIfOrb;
    IfCommunicator *m_pIfComm;
   
    TUInt16 m_nServiceId;


public:
    TUInt16 GetServiceId()
    {
        return m_nServiceId;
    }
    CService()
    {
        m_pClock = NULL;
        m_pIfOrb = NULL;
        m_pIfComm = NULL;
        m_nServiceId = 0;
    }
    virtual ~CService();
    TInt32 Init(TInt32 nInitTimerNr,CClock *pClock,IfCommunicator *pIfComm,IfOrb *pOrb,CDoid *pDoid);
    void *SetTimer(TUInt32 uGapInMs,TInt32 nRepeatTime,IfScheduler *pScheduler)
    {
        return m_tTimer.SetTimer(uGapInMs,nRepeatTime,pScheduler,m_pClock->GetPlatformTime());
    }
    //删除定时器pTimer是SetTimer返回的结果
    IfScheduler *KillTimer(void *pTimer)
    {
        return m_tTimer.KillTimer(pTimer);
    }

    const CClock *GetClock()
    {
        return m_pClock;
    }
    
    TInt32 RegisterSession(CSession *pSession);

};

}
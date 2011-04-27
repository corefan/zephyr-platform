#include "./DistributedObject.h"
#include "../../Public/include/TypeDef.h"
#include "../../Public/include/SysMacros.h"
#include "../../Public/include/Timer.h"
#include "../../Public/include/Clock.h"


namespace Zephyr
{

class CService : public CObject
{
private:
    CTimer m_tTimer;
    CClock  *m_pClock;
    
public:
    CService();
    virtual ~CService();
    TInt32 Init(TInt32 nInitTimerNr,CClock *pClock,IfCommunicator *pIfComm,COrb *pOrb,CDoid *pDoid);
    void *SetTimer(TUInt32 uGapInMs,TInt32 nRepeatTime,IfScheduler *pScheduler);
    //删除定时器pTimer是SetTimer返回的结果
    IfScheduler *KillTimer(void *pTimer);

    CClock *GetClock()
    {
        return m_pClock;
    }
};

}
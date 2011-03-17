#ifndef __ZEPHYR_SYSTEM_TIMER_TIMER_SYSTEM_H__
#define __ZEPHYR_SYSTEM_TIMER_TIMER_SYSTEM_H__

#include "../../../Public/include/TypeDef.h"
#include "../../../Public/Interface/Platform/include/IfTimer.h"

namespace Zephyr
{

class CSchedular
{
public:
    TUInt64             m_key;
    IfScheduler         *m_pScheduler;
    TplNode<CSchedular> *m_pListHeader;//如果指向自己，就是那个在map表里
};

class CTimerSystem : public IfTimer
{
public:
    TplMap<CSchedular>  m_tMaps;
    TUInt64             m_nLastRunTime;

public:
    CTimerSystem();
    TInt32 Init(int InitScheduler);
    //设置定时器，nGap是定的时常,IfScheduler是回调接口,
    void *SetTimer(TUInt32 uGapInMs,IfScheduler *pScheduler);

    //删除定时器pTimer是SetTimer返回的结果
    IfScheduler *KillTimer(void *pTimer);

    //Run是要定时去调用的，然后产生回调.
    TUInt32 Run(TUInt32 nRunCnt,TUInt32 nTimeNow);
private:

};

}

#endif

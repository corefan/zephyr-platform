#include "../include/TimerSystem.h"


//设置定时器，nGap是定的时常,IfScheduler是回调接口,
void *CTimerSystem::SetTimer(TUInt32 uGapInMs,IfScheduler *pScheduler)
{
    TUInt64 uT = m_nLastRunTime 
}

//删除定时器pTimer是SetTimer返回的结果
IfScheduler CTimerSystem::*KillTimer(void *pTimer)
{

}

//Run是要定时去调用的，然后产生回调.
TUInt32 CTimerSystem::Run(TUInt32 nRunCnt,TUInt32 nTimeNow)
{

}
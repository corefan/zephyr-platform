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
    TplNode<CSchedular> *m_pListHeader;//���ָ���Լ��������Ǹ���map����
};

class CTimerSystem : public IfTimer
{
public:
    TplMap<CSchedular>  m_tMaps;
    TUInt64             m_nLastRunTime;

public:
    CTimerSystem();
    TInt32 Init(int InitScheduler);
    //���ö�ʱ����nGap�Ƕ���ʱ��,IfScheduler�ǻص��ӿ�,
    void *SetTimer(TUInt32 uGapInMs,IfScheduler *pScheduler);

    //ɾ����ʱ��pTimer��SetTimer���صĽ��
    IfScheduler *KillTimer(void *pTimer);

    //Run��Ҫ��ʱȥ���õģ�Ȼ������ص�.
    TUInt32 Run(TUInt32 nRunCnt,TUInt32 nTimeNow);
private:

};

}

#endif

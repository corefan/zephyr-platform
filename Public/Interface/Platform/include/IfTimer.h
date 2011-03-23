/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfTimer.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: ��ʱ���ӿ�
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#define __ZEPHYR_IF_TIMER_H__
#define __ZEPHYR_IF_TIMER_H__

#include "../../include/TypeDef.h"

namespace Zephyr
{

class IfTimer
{
public:
    //���ö�ʱ����nGap�Ƕ���ʱ��,IfScheduler�ǻص��ӿ�,
    void *SetTimer(TUInt32 uGapInMs,IfScheduler *pScheduler) = 0;

    //ɾ����ʱ��pTimer��SetTimer���صĽ��
    IfScheduler *KillTimer(void *pTimer) = 0;

    //Run��Ҫ��ʱȥ���õģ�Ȼ������ص�.
    TUInt32 Run(TUInt32 nRunCnt,TUInt32 nTimeNow) = 0;
};

}

#endif

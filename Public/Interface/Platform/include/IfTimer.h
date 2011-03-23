/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfTimer.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 定时器接口
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
    //设置定时器，nGap是定的时常,IfScheduler是回调接口,
    void *SetTimer(TUInt32 uGapInMs,IfScheduler *pScheduler) = 0;

    //删除定时器pTimer是SetTimer返回的结果
    IfScheduler *KillTimer(void *pTimer) = 0;

    //Run是要定时去调用的，然后产生回调.
    TUInt32 Run(TUInt32 nRunCnt,TUInt32 nTimeNow) = 0;
};

}

#endif

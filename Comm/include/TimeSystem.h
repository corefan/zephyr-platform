#ifndef __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__
#define __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__

#include "TypeDef.h"


namespace Zephyr
{

class CTimeSystem
{
public:
    TUInt32 m_timeBegin; //系统开启时间
    volatile TUInt32 m_timeNow;
    //这个是全平台的统一时间，由Comm来同步，就是说无论在那个Comm上层，只要使用Comm都可能获得相同的m_nPlatformTime,希望能精确到40ms以内
    volatile TUInt64 m_nPlatformTime;
public:
    CTimeSystem();
    TUInt32 GetLocalTime()
    {
        return m_timeNow;
    }
    //注意，nLast必须是在本次调用之前获得的，返回的肯定是正数.
    TUInt32 GetTimeGap(TUInt32 nLast)
    {
        TUInt32 timeNow = m_timeNow; //一定要先存下来
        if (timeNow >= nLast)
        {
            return  timeNow - nLast;
        }
        else
        {
            return  ((((TUInt32)0xFFFFFFFF) - nLast) + timeNow);
        }
    }
    TUInt64 GetPlatformTime()
    {
        return m_nPlatformTime;
    }
    void Update();
};

}
#endif

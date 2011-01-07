#ifndef __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__
#define __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__

#include "TypeDef.h"


namespace Zephyr
{
class CTimeSystem
{
public:
    TUInt32 m_timeBegin;
    volatile TUInt32 m_timeNow;
public:
    CTimeSystem();
    TUInt32 GetTimeNow()
    {
        return m_timeNow;
    }
    void Update();
};

}
#endif

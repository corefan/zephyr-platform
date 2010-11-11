#ifndef __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__
#define __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__

#include "TypeDef.h"
#include "time.h"

namespace Zephyr
{
class CTimeSystem
{
public:
    TUInt32 m_timeBegin;
    TUInt32 m_timeNow;
public:
    TUInt32 GetTimeNow()
    {
        return m_timeNow;
    }
    void Update()
    {
        TUInt32 timeNow = (TUInt32)time(NULL);
        if (timeNow > m_timeBegin)
        {
            m_timeNow = timeNow - m_timeBegin;
        }
        else
        {
            m_timeNow = (((TUInt32)0xFFFFFFFF) - m_timeBegin) + timeNow;
        }
    }
};

}
#endif

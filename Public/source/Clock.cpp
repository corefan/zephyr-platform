#include "../include/Clock.h"

#ifdef _WIN32
//#include  <mmsystem.h>
#include <windows.h>

#else
#include<sys/time.h>
#endif

namespace Zephyr 
{

CClock::CClock()
{
#ifdef _WIN32
    m_timeBegin = (TUInt32)timeGetTime();
#else
    timeval * tv;
    gettimeofday(tv, 0 );
    m_timeBegin = tv->tv_usec + (tv->tv_sec * 1000);
#endif
    m_nPlatformTime = 0;
}
    
void CClock::Update()
{
#ifdef _WIN32
    TUInt32 timeNow = (TUInt32)timeGetTime();
#else
    timeval * tv;
    gettimeofday(tv, 0 );
    TUInt32 m_timeNow = (tv->tv_usec/1000) + (tv->tv_sec * 1000);
#endif
    int gap;
    if (timeNow >= m_timeNow) //这儿不用存，因为就是这儿更新的
    {
        gap = timeNow - m_timeNow;
    }
    else
    {
        gap = (((TUInt32)0xFFFFFFFF) - m_timeNow) + timeNow;
    }
    m_timeNow = timeNow;
    m_nPlatformTime += gap;
}
}

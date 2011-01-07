#include "TimeSystem.h"

#ifdef _WIN32
//#include  <mmsystem.h>
#include <windows.h>

#else
#include<sys/time.h>
#endif

namespace Zephyr 
{

CTimeSystem::CTimeSystem()
{
#ifdef _WIN32
    m_timeBegin = (TUInt32)timeGetTime();
#else
    timeval * tv;
    gettimeofday(tv, 0 );
    m_timeBegin = tv->tv_usec;
#endif

}
    
void CTimeSystem::Update()
{
#ifdef _WIN32
    TUInt32 timeNow = (TUInt32)timeGetTime();
#else
    timeval * tv;
    gettimeofday(tv, 0 );
    TUInt32 timeNow = tv->tv_usec;
#endif
    if (timeNow > m_timeBegin)
    {
        m_timeNow = timeNow - m_timeBegin;
    }
    else
    {
        m_timeNow = (((TUInt32)0xFFFFFFFF) - m_timeBegin) + timeNow;
    }
}
}

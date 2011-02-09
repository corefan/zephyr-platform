#ifndef __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__
#define __ZEPHYR_COMM_COMM_TIME_SYSTEM_H__

#include "TypeDef.h"


namespace Zephyr
{

class CTimeSystem
{
public:
    TUInt32 m_timeBegin; //ϵͳ����ʱ��
    volatile TUInt32 m_timeNow;
    //�����ȫƽ̨��ͳһʱ�䣬��Comm��ͬ��������˵�������Ǹ�Comm�ϲ㣬ֻҪʹ��Comm�����ܻ����ͬ��m_nPlatformTime,ϣ���ܾ�ȷ��40ms����
    volatile TUInt64 m_nPlatformTime;
public:
    CTimeSystem();
    TUInt32 GetLocalTime()
    {
        return m_timeNow;
    }
    //ע�⣬nLast�������ڱ��ε���֮ǰ��õģ����صĿ϶�������.
    TUInt32 GetTimeGap(TUInt32 nLast)
    {
        TUInt32 timeNow = m_timeNow; //һ��Ҫ�ȴ�����
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

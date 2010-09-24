#include "ProducerAndConsumer.h"
#include "SysMacros.h"

namespace Zephyr
{

CProduerAndConsumer::CProduerAndConsumer()
{
#ifdef _WIN32
    m_cond = CreateEvent(NULL,false,false,NULL);
#else
    m_seq = 0;
    m_confirmed = 0;
    pthread_mutex_init(&m_mutex,NULL);
    pthread_cond_init(&m_cond,NULL);
#endif`
}
//申请仓库空间
TBool    CProduerAndConsumer::OnProduced()
{
#ifdef _WIN32
    ++m_seq;
    return SetEvent(m_cond);
#else
    pthread_mutex_lock(&m_mutex);
    ++m_seq;
    bool ret = pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
    return ret;
#endif
}
//申请收货
//只有遍历所有生产者后才会锁.
TInt32 CProduerAndConsumer::RequireFetchProduct(TUInt32 timeout)
{
    TUInt32 seq = (TUInt32)m_seq;
    TUInt32 con = (TUInt32)m_confirmed;
    if (seq > con)
    {
        TUInt32 product = seq - con;
        ++m_confirmed;
        return product;
    }
    else if(seq < con)
    {
        TUInt32 product = (((TUInt32)0xFFFFFFFF) - con) + seq;
        ++m_confirmed;
        return product;
    }
    else
    {
    #ifdef _WIN32
        int ret = WaitForSingleObject(m_cond,timeout);
        if (WAIT_TIMEOUT == ret)
        {
            return TIME_OUT;
        }
        if (WAIT_OBJECT_0 == ret)
        {
            return SUCCESS;
        }
    #else

        struct timeval tv;
        struct timezone tz;


        if(gettimeofday(&tv,&tz)==0) {
            struct timespec ts;

            unsigned long long endtime = ((long long)tv.tv_sec) * 1000000 + tv.tv_usec+1000*timeout;

            ts.tv_sec = endtime/1000000;
            ts.tv_nsec = (endtime%1000000)*1000;

            pthread_mutex_lock(&m_mutex);
            //在这段小时间内，生产者又生产东西了！
            if (m_seq != m_confirmed)
            {
                pthread_mutex_unlock(&m_mutex);
                return RequireFetchProduct(timeout);
            }
            int retcode=pthread_cond_timedwait(&m_cond,&m_mutex,&ts);
            
            pthread_mutex_unlock(&m_mutex);
            return retcode;
        }
    #endif
    }
    return FAIL;
}


}


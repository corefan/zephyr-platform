#ifndef __ZEPHYR_SYSTEM_PRODUCER_AND_CONSUMER_H__
#define __ZEPHYR_SYSTEM_PRODUCER_AND_CONSUMER_H__
#include "Typedef.h"

#ifdef _WIN32

#else
#include <pthread.h>
#include <stdint.h>
#endif

namespace Zephyr
{

class CProduerAndConsumer
{
private:
    TUInt64 m_produced;
    TUInt64 m_consumered;
    TUInt64 m_depositoryCapacity;
    TUInt64 m_produceRequired;
    TUInt64 m_consumerRequired;
private:
    #ifndef _WIN32
    HANDLE  m_cond;
    #else //Linux
    pthread_cond_t m_cond;
    #endif
public:
    CProduerAndConsumer(TUInt64 depositoryCapacity);
    //����ֿ�ռ�
    TBool RequirDepositeProduct(TUInt32 productNr,TUInt32 timeout=15);
    //�������
    void ConfirmAddProduct(TUInt32 productNr);
    //�����ջ�
    TUInt32 RequireFetchProduct(TUInt32 timeout=15);
    //����Ҫ�����ջ�
    void OnConsumered(TUInt32 productionNr);
private:
    TInt32 CreateEvent();
    void SetEvent();
};

}

#endif

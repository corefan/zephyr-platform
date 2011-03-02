#include "IfTask.h"
#include "..\..\System\include\ProducerAndConsumer.h"
namespace Zephyr
{
IfTask::IfTask()
{
    m_pData = NULL;
    Init4Event();
}

IfTask::~IfTask()
{
    if (m_pData)
    {
        //�����
        delete m_pData;
        m_pData = NULL;
    }
}

void IfTask::Init4Event()
{
    if (!m_pData)
    {
        m_pData = new CProduerAndConsumer();
    }
}

TInt32  IfTask::Wait4Event(TInt32 timeout)
{
    return ((CProduerAndConsumer*)m_pData)->RequireFetchProduct(timeout);
}

void IfTask::OnNewEvent()
{
    ((CProduerAndConsumer*)m_pData)->OnProduced();
}


}

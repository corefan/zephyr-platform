#include "../include/Session.h"
#include "../include/Service.h"

namespace Zephyr
{
CSession::CSession()
{
    m_pService = NULL;
}

CSession::~CSession()
{
    
}
IfTimer *CSession::GetTimer()
{
    return m_pService->CService::GetTimer();
}
const CClock  *CSession::GetClock()
{
    return m_pService->CService::GetClock();
}

}
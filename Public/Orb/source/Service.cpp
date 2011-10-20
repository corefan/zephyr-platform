#include "../include/Service.h"

namespace Zephyr
{

CService::~CService()
{
    if (m_pIfOrb)
    {
        m_pIfOrb->UnRegisterObj(m_pSkeleton);
    }
}

//�������ж�pSession
TInt32 CService::RegisterSession(CSession *pSession)
{
    IfSkeleton *pSkeleton = m_pIfOrb->RegisterObj(pSession,m_nServiceId);
    if (pSkeleton)
    {
        pSession->SetSkeleton(pSkeleton);
        return SUCCESS;
    }
    return OUT_OF_MEM;
}

TInt32 CService::Init(const CClock *pClock,IfCommunicator *pIfComm,IfOrb *pOrb,CDoid *pDoid)
{
    m_pClock = pOrb->GetClock();
    m_pIfOrb = pOrb;
    m_nServiceId = pDoid->m_srvId;
    m_pIfComm = pIfComm;
	m_nServiceId = pDoid->m_srvId;
//     m_pSkeleton = pOrb->RegiterService(this,m_nServiceId);
//     if (NULL == m_pSkeleton)
//     {
//         return OUT_OF_MEM;
//     }
    return SUCCESS;
}

}
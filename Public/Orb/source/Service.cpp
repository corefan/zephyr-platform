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

//调用者判定pSession
TInt32 CService::RegisterSession(CSession *pSession)
{
    TInt32 nRet = pSession->OnInit();
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    IfSkeleton *pSkeleton = m_pIfOrb->RegisterObj(pSession,m_nServiceId);
    if (pSkeleton)
    {
        pSession->SetSkeleton(pSkeleton);
        nRet = pSession->OnInited();
        if (nRet < SUCCESS)
        {
            m_pIfOrb->UnRegisterObj(pSkeleton);
            return nRet;
        }
        return SUCCESS;
    }
    return OUT_OF_MEM;
}

void CService::UnRegisterSession(CSession *pSession)
{
    pSession->OnFinal();
    m_pIfOrb->UnRegisterObj(pSession->GetSkeleton());
    pSession->SetSkeleton(NULL);
    pSession->OnFinaled();
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
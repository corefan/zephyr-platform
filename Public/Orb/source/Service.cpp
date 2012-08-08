#include "../include/Service.h"
#include "../../../System/include/Lock.h"
namespace Zephyr
{
CService::CService()
{
    m_pClock = NULL;
    m_pIfOrb = NULL;
    m_pIfComm = NULL;
    m_pLock = new CLock();
    m_nServiceId = 0;
}

CService::~CService()
{
    if (m_pIfOrb)
    {
        m_pIfOrb->UnRegisterObj(m_pSkeleton);
    }
    if (m_pLock)
    {
        DELETEP(m_pLock);
    }
}

void CService::LockInterService()
{
    ((CLock*)m_pLock)->Lock();
}
void CService::UnlockInterService()
{
    ((CLock*)m_pLock)->Unlock();
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

 TInt32  CService::OnRoutine(TUInt32 nRunCnt)
 {
    return m_tTimer.Run(m_pClock->GetPlatformTime());
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
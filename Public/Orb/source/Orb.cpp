#include "../include/Orb.h"
#include "../../include/SysMacros.h"
#include <iostream>
#include "../../../System/include/SysInc.h"
namespace Zephyr
{

IfSkeleton* COrb::RegisterObj(IfObj *pObjSkeleton,TInt16 nSrvId)
{
    if ((nSrvId >= m_nLocalServiceId) && (nSrvId < (m_nLocalServiceIdEnd)))
    {
        CListNode<CArrayPoolNode<CSkeleton> >  *pRtn = m_tSkeletonPool.GetMem();
        if (pRtn)
        {
            pRtn->m_pListBelongsTo = NULL;
            pRtn->m_tDoid.m_srvId = nSrvId;
        }
        
        return pRtn;
    }
    return NULL;
}

IfSkeleton* COrb::RegiterService(IfObj *pObj,TInt16 nSrvId)
{
    if ((nSrvId >= m_nLocalServiceId) && (nSrvId < (m_nLocalServiceIdEnd)))
    {
        if (m_ppServiceSkeleton[nSrvId-m_nLocalServiceId])
        {
            return NULL;
        }
        CListNode<CArrayPoolNode<CSkeleton> >  *pRtn = m_tSkeletonPool.GetMem();
        if (pRtn)
        {
            pRtn->m_pListBelongsTo = NULL;
            pRtn->m_tDoid.m_srvId = nSrvId;
            m_ppServiceSkeleton[nSrvId-m_nLocalServiceId] = pRtn;
        }
        return pRtn;
    }
    return NULL;
}

    //注销
void    COrb::UnRegisterObj(IfSkeleton *pIfSkel)
{
    CListNode<CArrayPoolNode<CSkeleton> > *pSk = (CListNode<CArrayPoolNode<CSkeleton> >*)pIfSkel;
    pSk->OnReused(m_tSkeletonPool.GetMaxSize());
    m_tSkeletonPool.ReleaseMem(pSk);
}

TInt32 COrb::Init(IfCommunicator *pIfCom,CDoid *pDoidBegin,TInt32 nSkeletonNr)
{
    m_pClock = pIfCom->GetClock();
    if (!m_pClock)
    {
        printf("Can not get clock!");
        return NULL_POINTER;
    }
    m_nLastCheckTime = m_pClock->GetLocalTime();
    m_pIfComm = pIfCom;

    m_nLocalNodeId = pDoidBegin->m_nodeId;
    m_nLocalVIP = pDoidBegin->m_virtualIp;
    //这个Orb的service从这个开始
    m_nLocalServiceId = pDoidBegin->m_srvId;
    for (int i=0;i<MAX_SERVICE_NR;++i)
    {
        m_ppServiceSkeleton[i] = NULL;
    }
    

    m_nLocalServiceIdEnd = m_nLocalServiceId + MAX_SERVICE_NR_PER_COMM;
    TInt32 nRet = m_tSkeletonPool.InitPool(nSkeletonNr);
    if (nRet < SUCCESS)
    {
        return nRet;
    }
    for (int i=0;i<nSkeletonNr;++i)
    {
        CSkeleton *pSkt = m_tSkeletonPool.FindMem(i);
        CDoid *pDoid = pSkt->GetMyDoid();
        pDoid->m_nodeId = m_nLocalNodeId;
        pDoid->m_virtualIp = m_nLocalVIP;
        pDoid->m_srvId = m_nLocalServiceId;
        pDoid->m_objId = i;
    }
}

void COrb::StopService(TUInt32 nServiceID)
{
    if (0xFFFFFFFF != nServiceID)
    {
        m_nService2Stop = nServiceID;
        while (m_nService2Stop == nServiceID) //改为-1
        {
            SleepT(15); //不停的尝试.
        }
    }
}


TInt32 COrb::RegisterRun(IfSkeleton *pIfSkeleton,TUInt32 nGapInMs)
{
    CListNode<CArrayPoolNode<CSkeleton> > *pSk = (CListNode<CArrayPoolNode<CSkeleton> >*)pIfSkeleton;
    if (pSk->m_pListBelongsTo)
    {
        pSk->m_pListBelongsTo->Detach(pSk);
        pSk->m_pListBelongsTo = NULL;
    }
    m_tRunning.push_back(pSk);
    pSk->m_pListBelongsTo = &m_tRunning;
    return SUCCESS;
}
    //时间相关
const CClock *COrb::GetClock()
{
    return m_pClock;
}


TInt32 COrb::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int nUsedCnt = 0;
    CMessageHeader *pMsg = m_pIfComm->GetMsg();
    while (pMsg)
    {
        CDoid *pDest = pMsg->GetDestDoidByIdx();
        CListNode<CArrayPoolNode<CSkeleton> > *pSk = m_tSkeletonPool.FindMem(pDest->m_objId);
        if (pSk->m_pListBelongsTo != &m_tSkeletonPool.m_tFree)
        {
            if (pSk->m_tDoid == *pDest)
            {
                pSk->OnRecv(pMsg);
            }
        }
        for (int i=1;i<=pMsg->GetBroadcastDoidNr();++i)
        {
            pDest = pMsg->GetDestDoidByIdx(i);
            CListNode<CArrayPoolNode<CSkeleton> > *pSk = m_tSkeletonPool.FindMem(pDest->m_objId);
            if (pSk->m_pListBelongsTo != &m_tSkeletonPool.m_tFree)
            {
                if (pSk->m_tDoid == *pDest)
                {
                    pSk->OnRecv(pMsg);
                }
            }
        }
        ++nUsedCnt;
        m_pIfComm->ReturnMsgBuff(pMsg);
        if (nUsedCnt > runCnt)
        {
            break;
        }
        else
        {
            pMsg = m_pIfComm->GetMsg();
        }
    }

    return SUCCESS;
}

IfCommunicator *COrb::GetCommunicator()
{
    return m_pIfComm;
}

}
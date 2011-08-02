#include "../include/RouteMap.h"
#include "Public/include/Message.h"
#include "../include/GatewayLogger.h"

namespace Zephyr
{


CDoid *CRouteMap::FindService(TUInt32 uServiceId)
{
    TUInt32 uSvr = CMessageHeader::GetServiceID(uServiceId);
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSvr);
    CRoute *pRount = it;
    CRoute *pRtn(NULL);
    while ((pRount))
    {
        if ((uServiceId <= pRount->m_uIdBegin)&&(uServiceId> pRount->m_uIdEnd))
        {
            if (pRtn)
            {
                if (pRtn->m_uPriority > pRount->m_uPriority) //计算权重.
                {
                    pRtn = pRount;
                }
            }
            else
            {
                pRtn = pRount;
            }
        }
        ++it;
    }
    if (pRtn)
    {
        return &pRtn->m_tRouteTo;
    }
    return NULL;
}

TInt32 CRouteMap::AddRoute(CDoid *pDoid,CDoid *pRegister,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd,TUInt32 uPriority)
{
    if ((uSrvId != CMessageHeader::GetServiceID(uBegin))||(uSrvId!=CMessageHeader::GetServiceID(uEnd-1)))
    {
        //入参错误，写日志
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        char szBufferDoid[64];
        pDoid->ToStr(szBufferDoid);
        LOG_RUN(en_error_service_id,"Error Servcie Id,Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
        return FAIL;
    }
    //查找有没有，没的话再说.
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSrvId);
    CRoute *pRount = it;
    if (pRount)
    {
        CRoute tNew;
        tNew.m_tRouteTo = *pDoid;
        tNew.m_uIdBegin = uBegin;
        tNew.m_uIdEnd   = uEnd;
        tNew.m_uKey     = uSrvId;
        tNew.m_uPriority = uPriority;
        CRoute *pMerge = &tNew;
        while ((pRount))
        {
            if (pRount->Merge(*pMerge)) //成功融合
            {
                if (&tNew == pMerge)
                {
                    pMerge = pRount;
                }
                else
                {
                    //把老的删掉.因为已经融到pRount里了
                    m_tServiceRoute.RemoveFromTreeItem(pMerge);
                    pMerge = pRount;
                }
            }
            ++it;
        }
        if (&tNew != pMerge)
        {
            //好了，数据已经插入了，可以返回了
            return SUCCESS;
        }
    }

    CRoute *pRoute = m_tServiceRoute.PrepareItem();
    if (pRoute)
    {
        pRoute->m_uKey = uSrvId;//CMessageHeader::GetServiceID();不用了
        pRoute->m_uIdBegin = uBegin;
        pRoute->m_uIdEnd = uEnd;
        pRoute->m_tRouteTo = *pDoid;
        pRoute->m_uPriority = uPriority;
        m_tServiceRoute.AddInTree(pRoute);
    }
    else
    {
        //写日志.内存
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        char szBufferDoid[64];
        pDoid->ToStr(szBufferDoid);
        LOG_CRITICAL(en_allocate_route_mem_failed,"Servcie Id,Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
    }
    return SUCCESS;
}

void CRouteMap::OnFinal()
{
    m_tServiceRoute.clean();
}

}
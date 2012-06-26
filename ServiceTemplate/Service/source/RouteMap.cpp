#include "../include/RouteMap.h"
#include "Public/include/Message.h"
#include "../include/GatewayLogger.h"
#include "../../Interface/include/IfConnectingRespStub.h"
namespace Zephyr
{


CDoid *CRouteMap::FindService(TUInt32 uServiceId)
{
    TUInt32 uSvr = CMessageHeader::GetServiceID(uServiceId);
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSvr);
    CRoute *pRount = it;
    CRoute *pRtn(NULL);
    while ((pRount)&&(pRount->m_uKey == uSvr))
    {
        if ((uServiceId >= pRount->m_uIdBegin)&&(uServiceId < pRount->m_uIdEnd))
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

TInt32 CRouteMap::ChangePriorty(TUInt32 uServiceId,CDoid *pMyDoid,TUInt32 uPriority)
{
    TUInt32 uSvr = CMessageHeader::GetServiceID(uServiceId); //修改一个Service的
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSvr);
    CRoute *pRount = it;
    TInt32 nCount = 0;
    while ((pRount)&&(pRount->m_uKey == uSvr))
    {
        if (pRount->m_tRouteTo == (*pMyDoid))
        {
            pRount->m_uPriority = uPriority;
            ++nCount;
        }
        ++it;
    }
    return nCount;
}

TInt32 CRouteMap::AddRoute(CDoid *pDoid,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd,TUInt32 uPriority)
{
    if ((uSrvId != CMessageHeader::GetServiceID(uBegin))||(uSrvId!=CMessageHeader::GetServiceID(uEnd-1)))
    {
        //入参错误，写日志
//         char szBufferRegister[64];
//         pRegister->ToStr(szBufferRegister);
//         char szBufferDoid[64];
//         pDoid->ToStr(szBufferDoid);
//         LOG_RUN(en_error_service_id,"Error Servcie Id,Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
        return -((TInt32)en_error_service_id);
    }
    //查找有没有，没的话再说.
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSrvId);

    if ((CRoute *)it)
    {
        while (((CRoute *)it)&&(it->m_uKey == uSrvId))
        {
            if (it->m_tRouteTo == *pDoid) //有老的
            {
                //则替换老的
                it->m_uIdBegin = uBegin;
                it->m_uIdEnd   = uEnd;
                it->m_uPriority= uPriority;
                return SUCCESS;
            }
            ++it;
        }
    }
    else
    {
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
            //         char szBufferRegister[64];
            //         pRegister->ToStr(szBufferRegister);
            //         char szBufferDoid[64];
            //         pDoid->ToStr(szBufferDoid);
            //         LOG_CRITICAL(en_allocate_route_mem_failed,"Servcie Id,Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
            return OUT_OF_MEM;
        }
    }
    return SUCCESS;
}

TInt32 CRouteMap::RmvRoute(CDoid *pDoid,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd)
{
    if ((uSrvId != CMessageHeader::GetServiceID(uBegin))||(uSrvId!=CMessageHeader::GetServiceID(uEnd-1)))
    {
        //入参错误，写日志
        //         char szBufferRegister[64];
        //         pRegister->ToStr(szBufferRegister);
        //         char szBufferDoid[64];
        //         pDoid->ToStr(szBufferDoid);
        //         LOG_RUN(en_error_service_id,"Error Servcie Id,Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
        return -((TInt32)en_error_service_id);
    }

    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.GetItemByKey(uSrvId);
    TInt32 nResult = -((TInt32)en_no_much_service_id_found);
    if ((CRoute*)it)
    {
        while (((CRoute*)it)&&(it->m_uKey == uSrvId))
        {
            if (it->m_tRouteTo == *pDoid)
            {
                m_tServiceRoute.RemoveFromTreeItem(it);
                m_tServiceRoute.ReleaseItem(it);
            }
            ++it;
        }
    }
    return nResult;
}

void CRouteMap::ReleaseAndInfoRegister(IfSkeleton *pSkeleton,TUInt32 nReason)
{
    TplMultiKeyMapNode<CRoute,TUInt32>::Iterator it = m_tServiceRoute.Begin();
    TplMultiKeyMapNode<CRoute,TUInt32> *pNode = it;

    IfConnectingRespStub tStub;
    while(pNode)
    {
        ++it;
        tStub.Init(pSkeleton,&pNode->m_tRouteTo);
        tStub.OnDisconnect(nReason);
        m_tServiceRoute.RemoveFromTreeItem(pNode);
        m_tServiceRoute.ReleaseItem(pNode);
        pNode = it;
    }
}

void CRouteMap::OnFinal()
{
    m_tServiceRoute.clean();
}

}
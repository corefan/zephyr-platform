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
                if (pRtn->m_uPriority > pRount->m_uPriority) //����Ȩ��.
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
        //��δ���д��־
        char szBufferRegister[64];
        pRegister->ToStr(szBufferRegister);
        char szBufferDoid[64];
        pDoid->ToStr(szBufferDoid);
        LOG_RUN(en_error_service_id,"Error Servcie Id,Register:%s ,doid:%s, uSrvId:%u,uBegin:%u,uEnd:%u,uPriority:%u",szBufferRegister,szBufferDoid,uSrvId,uBegin,uEnd,uPriority);
        return FAIL;
    }
    //������û�У�û�Ļ���˵.
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
            if (pRount->Merge(*pMerge)) //�ɹ��ں�
            {
                if (&tNew == pMerge)
                {
                    pMerge = pRount;
                }
                else
                {
                    //���ϵ�ɾ��.��Ϊ�Ѿ��ڵ�pRount����
                    m_tServiceRoute.RemoveFromTreeItem(pMerge);
                    pMerge = pRount;
                }
            }
            ++it;
        }
        if (&tNew != pMerge)
        {
            //���ˣ������Ѿ������ˣ����Է�����
            return SUCCESS;
        }
    }

    CRoute *pRoute = m_tServiceRoute.PrepareItem();
    if (pRoute)
    {
        pRoute->m_uKey = uSrvId;//CMessageHeader::GetServiceID();������
        pRoute->m_uIdBegin = uBegin;
        pRoute->m_uIdEnd = uEnd;
        pRoute->m_tRouteTo = *pDoid;
        pRoute->m_uPriority = uPriority;
        m_tServiceRoute.AddInTree(pRoute);
    }
    else
    {
        //д��־.�ڴ�
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
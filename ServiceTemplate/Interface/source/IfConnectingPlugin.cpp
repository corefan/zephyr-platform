#include "../include/IfConnectingPlugin.h"
#include "../include/IfConnectingStub.h"

namespace Zephyr
{

void CIfConnectingPlugin::OnInit()
{
    m_tRegisteredDoid = NULL_DOID;
    m_uServiceId= 0;
    m_uServiceBegin= 0;
    m_uServiceIdEnd= 0;
    m_uPrioirty = 0;
    m_enState= en_inited;
}

TInt32 CIfConnectingPlugin::RespRegisterService(CDoid *pDoid,TUInt32 uServiceId,TInt32 nRslt)
{
    if (en_registering == m_enState)
    {
        m_enState = en_registered;
        if (m_pTimer)
        {
            m_pOwner->GetTimer()->KillTimer(m_pTimer);
            m_pTimer = NULL;
        }
        if (m_pCallback)
        {
            m_pCallback->RespRegisterService(pDoid,uServiceId,nRslt);
        }
    }
    //all the other state, we did not handle them,
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::ConfirmUnregisterService(TUInt32 uServiceId)
{    
    if (en_unregistering == m_enState)
    {
        m_enState = en_unregistered;
        if (m_pTimer)
        {
            m_pOwner->GetTimer()->KillTimer(m_pTimer);
            m_pTimer = NULL;
        }
        if (m_pCallback)
        {
            m_pCallback->ConfirmUnregisterService(uServiceId);
        }
    }
    //all the other state, we did not handle them,
    return SUCCESS;
}
//×¢²á¹ã²¥

TInt32 CIfConnectingPlugin::OnDisconnect(TUInt32 uReason)
{
    //change the 
    if (*m_pOwner->GetCallerDoid() == m_tGwSeDoid)
    {
        m_enState = en_disconnected;
        //if (en_disconnecting == m_enState)
        {
            if (m_pTimer)
            {
                m_pOwner->GetTimer()->KillTimer(m_pTimer);
            }
            if (m_pCallback)
            {
                m_pCallback->OnDisconnect(uReason);
            }
        }
    }
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::SendRSAPublicKey(TUInt16 uBits,TUInt16 isFermat4,OctSeq<TUInt16> e,OctSeq<TUInt16> n,TUInt32 uDyncNr)
{
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::OnScheduler(TUInt64 nTimeNow,void *pTimer,TInt32 nRepeatTimeRemains)
{
    m_pTimer = NULL;
    switch (m_enState)
    {
    case en_inited:
        {
            //this should not happened!
        }
        break;
    case en_registering:
        {
            RegisterService();
        }
        break;
    case en_registered:
        {
            //this should not happened
        }
        break;
    case en_unregistering:
        {
            UnregisterService();
        }
        break;
    case en_unregistered:
        {
            //this should not thappened;
        }
        break;
    case en_disconnecting:
        {
            Disconnect();
        }
        break;
    case en_disconnected:
        {
            //this should not happened!
        }
        break;
    }
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::RegisterService(CDoid* pDoid,TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,TUInt32 uPriority)
{
    if (en_inited == m_enState)
    {
        m_tRegisteredDoid = *pDoid;
        m_uServiceBegin = uServiceIdBegin;
        m_uServiceIdEnd = uServcieIdEnd;
        if (CMessageHeader::GetServiceID(m_uServiceBegin) != CMessageHeader::GetServiceID(m_uServiceIdEnd))
        {
            return OUT_OF_RANGE;
        }
        m_uPrioirty = uPriority;
        if (m_pTimer)
        {
            m_pTimer = m_pOwner->GetTimer()->ResetTimer(m_pTimer,RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
        }
        else
        {
            m_pTimer = m_pOwner->GetTimer()->SetTimer(RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
        }
        if (NULL == m_pTimer)
        {
            return OUT_OF_MEM;
        }
        m_enState = en_registering;
        return RegisterService();
    }
    return INCORRECT_STATE;
}

TInt32 CIfConnectingPlugin::UnregisterService(TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd)
{
    if ((en_unregistering > m_enState)&&(m_enState < en_inited))
    {
        m_enState = en_unregistering;
        return UnregisterService();
    }
    return INCORRECT_STATE;
}

TInt32 CIfConnectingPlugin::Disconnect(TUInt32 uReason)
{
    m_uPrioirty = uReason; //record it here!
    if (m_enState < en_disconnecting)
    {
        m_enState = en_disconnecting;
        return Disconnect();
    }
    return SUCCESS;
}

void CIfConnectingPlugin::OnFinal()
{
    //²»¹Ü×´Ì¬
    if (m_pTimer)
    {
        m_pOwner->GetTimer()->KillTimer(m_pTimer);
        m_pTimer = NULL;
    }
    m_pCallback = NULL;
}

TInt32 CIfConnectingPlugin::RegisterService()
{
    if (m_pTimer)
    {
        m_pTimer = m_pOwner->GetTimer()->ResetTimer(m_pTimer,RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
    }
    else
    {
        m_pTimer = m_pOwner->GetTimer()->SetTimer(RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
    }
    if (NULL == m_pTimer)
    {
        return OUT_OF_MEM;
    }
    CDoid tGw = m_tGwSeDoid;
    tGw.m_objId = 0;
    IfConnecting *pGw;
    GET_REMOTE_STUB_BY_DOID(pGw,IfConnecting,tGw);
    return pGw->RegisterService(&m_tRegisteredDoid,CMessageHeader::GetServiceID(m_uServiceBegin),m_uServiceBegin,m_uServiceIdEnd,m_uPrioirty);
}

TInt32 CIfConnectingPlugin::UnregisterService()
{
    if (m_pTimer)
    {
        m_pTimer = m_pOwner->GetTimer()->ResetTimer(m_pTimer,RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
    }
    else
    {
        m_pTimer = m_pOwner->GetTimer()->SetTimer(RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
    }
    if (NULL == m_pTimer)
    {
        return OUT_OF_MEM;
    }
    CDoid tGw = m_tGwSeDoid;
    tGw.m_objId = 0;
    IfConnecting *pGw;
    GET_REMOTE_STUB_BY_DOID(pGw,IfConnecting,tGw);
    return pGw->UnregisterService(CMessageHeader::GetServiceID(m_uServiceBegin),m_uServiceBegin,m_uServiceIdEnd);
}

TInt32 CIfConnectingPlugin::Disconnect()
{
    if (m_pTimer)
    {
        m_pTimer = m_pOwner->GetTimer()->ResetTimer(m_pTimer,RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
    }
    else
    {
        m_pTimer = m_pOwner->GetTimer()->SetTimer(RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pOwner->GetClock()->GetPlatformTime());
    }
    if (NULL == m_pTimer)
    {
        return OUT_OF_MEM;
    }
    CDoid tGw = m_tGwSeDoid;
    tGw.m_objId = 0;
    IfConnecting *pGw;
    GET_REMOTE_STUB_BY_DOID(pGw,IfConnecting,tGw);
    return SUCCESS;
}
}
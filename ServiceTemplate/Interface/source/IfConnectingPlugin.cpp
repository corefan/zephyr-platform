#include "../include/IfConnectingPlugin.h"

namespace Zephyr
{

TInt32 CIfConnectingPlugin::OnScheduler(TUInt64 nTimeNow,void *pTimer,TInt32 nRepeatTimeRemains)
{
    switch (m_enState)
    {
    case en_inited:
        {
            //this should not happened!
        }
        break;
    case en_registering:
        {
        }
        break;
    case en_registered:
        {

        }
        break;
    case en_unregistering:
        {
        }
        break;
    case en_unregistered:
        {
        }
        break;
    case en_disconnecting:
        {
        }
        break;
    case en_disconnected:
        {
        }
        break;
    }
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::RegisterService(CDoid* pDoid,TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd,TUInt32 uPriority)
{
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::UnregisterService(TUInt32 uServiceId,TUInt32 uServiceIdBegin,TUInt32 uServcieIdEnd)
{
    return SUCCESS;
}

TInt32 CIfConnectingPlugin::Disconnect(TUInt32 uReason)
{
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
}


}
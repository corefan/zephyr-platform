#ifndef __IFlOGICSERVICE_PLUGIN_H__
#define __IFlOGICSERVICE_PLUGIN_H__

#include "Public/include/TypeDef.h"
#include "../interface/IfLogicServiceResp.h"
#include "Public/include/SysMacros.h"
#include "IfLogicServiceStub.h"
namespace test_logic
{

class IfLogicServicePlugin : public IfLogicServiceResp
                           , public IfScheduler
{
private:
    enum EnState
    {
        en_inited,
        en_try_get_session,
        en_session_created,
        en_releasing,
        en_released,
    };
    CObject         *m_pObject;
    CDoid           m_tServiceDoid;
    TUInt64         m_uId;
    CDoid           m_tGwDoid;
    OctSeq<TUInt16> m_tData;
    TUInt16         m_enState;
    void            *m_pTimer;
    IfLogicServiceResp *m_pUser;
public:
    IfLogicServicePlugin()
    {
        m_pObject = NULL;
        m_uId = 0;
        m_enState = en_inited;
        m_pTimer = NULL;
        m_pUser = NULL;
    }
    void Init(CDoid tServiceDoid,CObject *pObject,IfLogicServiceResp *pUser)
    {
        m_pObject = pObject;
        m_tGwDoid = tServiceDoid;
        m_enState = en_inited;
        m_pTimer  = NULL;
        m_pUser = pUser;
    }
    TInt32 ReInit(CDoid tServiceDoid)
    {
        if ((en_released == m_enState)||(en_inited == m_enState))
        {
            m_enState = en_inited;
            m_pTimer  = NULL;
        }
        return SESSION_STATE_INCORRECT;
    }
    TUInt16 GetState()
    {
        return m_enState;
    }
    CDoid GetServiceDoid()
    {
        CDoid t(m_tServiceDoid.m_nodeId,m_tServiceDoid.m_virtualIp,m_tServiceDoid.m_srvId,0);
        return t;
    }
    TInt32 ReqGetSession(TUInt64 uId,CDoid tGwDoid,OctSeq<TUInt16> tData)
    {
        if (en_inited == m_enState)
        {
            m_uId = uId;
            m_tGwDoid = tGwDoid;
            m_tData.m_nBodyLength = tData.m_nBodyLength;
            m_tData.m_pBuffer = tData.m_pBuffer; //maybe we should new memory here
        }
        return ALREADY_CREATE_SESSION;
    }
    TInt32 ReqReleaseSession(TUInt64 uId,CDoid tSess,OctSeq<TUInt16> tData)
    {
        if (m_enState > en_try_get_session)
        {
            
        }
        return SUCCESS;
    }
    virtual TInt32 RespGetSession(TUInt64 uId,CDoid tSessDoid)
    {
        if (m_uId == uId)
        {
            if (en_try_get_session == m_enState)
            {
                KillTimer();
                m_enState = en_session_created;
                m_tServiceDoid = tSessDoid;
                if (m_pUser)
                {
                    m_pUser->RespGetSession(m_uId,tSessDoid);
                }
            }
            //ÊÇÒ»¸ö
        }
        return SUCCESS;
    }
    virtual TInt32 RespReleaseSession(TUInt64 uId)
    {
        if (m_uId == uId)
        {
            if (en_releasing == m_enState)
            {
                KillTimer();
                m_enState = en_released;
                if (m_pUser)
                {
                    m_pUser->RespReleaseSession(uId);
                }
            }
        }
        return SUCCESS;
    }
    virtual TInt32 OnScheduler(TUInt64 nTimeNow,void *pTimer,TInt32 nRepeatTimeRemains)
    {
        switch(m_enState)
        {
        case en_try_get_session:
            {
                TryGetSession();
            }
            break;
        case en_releasing:
            {
                TryReleaseSession();
            }
            break;
        }
        return SUCCESS;
    }

private:
    TInt32 TryGetSession()
    {
        SetTimer();
        if (m_pTimer)
        {
            IfLogicServiceStub tStub;
            tStub.Init(m_pObject->GetSkeleton(),&m_tServiceDoid);

            TInt32 nRet = tStub.ReqGetSession(m_uId,m_tGwDoid,m_tData);
            if (nRet < SUCCESS)
            {
                KillTimer();
                return nRet;
            }
            m_enState = en_try_get_session;
            return SUCCESS;
        }
        else
        {
            return OUT_OF_MEM;
        }
    }
    TInt32 TryReleaseSession()
    {
        if ((en_try_get_session <= m_enState) && (en_released > m_enState))
        {
            SetTimer();
            if (m_pTimer)
            {
                IfLogicServiceStub tStub;
                CDoid tService = m_tServiceDoid;
                tService.m_objId = 0;
                tStub.Init(m_pObject->GetSkeleton(),&tService);
                TInt32 nRet = tStub.ReqReleaseSession(m_uId,m_tServiceDoid,m_tData);
            }
            return OUT_OF_MEM;
        }
        else if (en_released == m_enState)
        {
            if (m_pUser)
            {
                m_pUser->RespReleaseSession(m_uId);
            }
        }
        return SUCCESS;
    }
    void KillTimer()
    {
        if (m_pTimer)
        {
            m_pObject->GetTimer()->KillTimer(m_pTimer);
            m_pTimer = NULL;
        }
    }
    void SetTimer()
    {
        if (m_pTimer)
        {
            m_pTimer = m_pObject->GetTimer()->ResetTimer(m_pTimer,RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pObject->GetClock()->GetPlatformTime());
        }
        else
        {
            m_pTimer = m_pObject->GetTimer()->SetTimer(RETRY_REMOTE_CALL_TIME_IN_MS,0,this,m_pObject->GetClock()->GetPlatformTime());
        }
    }
};

}

#endif
 
#ifndef __ZEPHYR_COMM_NET_NET_EVENT_QUEUE_H__
#define __ZEPHYR_COMM_NET_NET_EVENT_QUEUE_H__\



#include "TypeDef.h"
#include "SysMacros.h"
#include "Pipe.h"
#include "IfTask.h"
#include "stdio.h"


namespace Zephyr
{

struct TIOEvent
{
    TUInt16                     m_connectionIdx;
    TUInt16                     m_connectionEvents;
    TUInt32                     m_seqNum;
};


class CIoEventQueue
{
private:
    CPipe   m_pipe;
public:
    TInt32  Init(TInt32 maxEventNr)
    {
        return m_pipe.Init(maxEventNr * sizeof(TIOEvent));
    }
    TInt32  AddEvent(TIOEvent &event)
    {
        TUInt32 len = sizeof(TIOEvent);
        TUChar *pMsg = m_pipe.PrepareMsg(len);
        if (pMsg)
        {
            //不应该！
            if (len < sizeof(TIOEvent))
            {
                printf("Something wrong?!");
                return NOT_INITIALIZED;
            }
            memcpy(pMsg,&event,sizeof(TIOEvent));
            m_pipe.ConfirmAddMsg(pMsg,sizeof(TIOEvent));
            return SUCCESS;
        }
        return OUT_OF_MEM;
    }
    TIOEvent *GetEvent()
    {
        TUInt32 len = sizeof(TIOEvent);
        TUChar *pData = m_pipe.GetData(len);
        if (pData)
        {
            if (len < sizeof(TIOEvent))
            {
                return NULL;
            }
        }
        return (TIOEvent*)pData;
    }
    void ConfirmHandleEvent(TIOEvent *pEvent)
    {
        m_pipe.ReturnMsgBuff((TUChar*)pEvent,sizeof(TIOEvent));
    }
};

class CNetEventQueues
{
private:
    CIoEventQueue m_netQueue;
    CIoEventQueue m_appQueue;
    IfTask        *m_pApp;
public:
    TInt32      Init(TInt32 maxEventNr,IfTask *pIfTask)
    {
        m_pApp = pIfTask;
        TInt32 rtn = m_netQueue.Init(maxEventNr);
        if (SUCCESS > rtn)
        {
            //这儿就不管内存分配失败后的处理了，应该都成功的.失败了就把程序关了
            return rtn;
        }
        return m_appQueue.Init(maxEventNr);
    }
    TInt32      AddNetEvent(TIOEvent &event)
    {
        TInt32 rtn = m_netQueue.AddEvent(event);
        if (m_pApp)
        {
            m_pApp->OnNewEvent();
        }
        return rtn;
    }
    TIOEvent    *GetNetEvent()
    {
        return m_netQueue.GetEvent();
    }
    void        ConfirmHandleNetEvent(TIOEvent *pEvent)
    {
        m_netQueue.ConfirmHandleEvent(pEvent);
    }

    TInt32      AddAppEvent(TIOEvent &event)
    {
        return m_appQueue.AddEvent(event);
    }
    TIOEvent    *GetAppEvent()
    {
        return m_appQueue.GetEvent();
    }
    void        ConfirmHandleAppEvent(TIOEvent *pEvent)
    {
        m_appQueue.ConfirmHandleEvent(pEvent);
    }
};


};


#endif

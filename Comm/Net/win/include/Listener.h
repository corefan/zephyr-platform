/*-------------------------------------------------------------
 Copyright (C)| 
 File: Listener.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  ¼àÌýÆ÷
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_LISTENER_H__
#define __ZEPHYR_LISTENER_H__

#include "TypeDef.h"
#include "SysMacros.h"
#include "winsock2.h"

namespace Zephyr
{
    
class CListener
{
private:
    TUInt32     m_myIp;
    TUInt16     m_listeningPort;
    TUInt16     m_maxAcceptNr;
    SOCKET      m_listeningSocket;
    void        *m_pAppData;
    CListener   *m_pNext;
public:
    CListener();
    ~CListener();
    TInt32 Init(TUInt32 myIp,TUInt16 listeningPort,TUInt16 maxAcceptNr,void *pAppData);
    
    SOCKET Accept();
    TInt32 HasNewConnection();
    void *GetAppData()
    {
        return m_pAppData;
    }
    void AttachToList(CListener *pLast)
    {
        m_pNext =  pLast;
    }
    CListener *GetNext()
    {
        return m_pNext;
    }
};

}
#endif
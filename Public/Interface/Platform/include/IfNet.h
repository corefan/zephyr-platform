/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfNet.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  ´«Êä²ã½Ó¿Ú
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_IF_NET_H__
#define __ZEPHYR_IF_NET_H__

#include "TypeDef.h"
#include "SysMacros.h"
#include "IfNetData.h"

namespace Zephyr
{

class IfNet
{
public:
    virtual TInt32 SendMsg(TInt32 toConnectionIdx,TUChar *pMsg,TUInt32 msgLen) = 0;
    virtual TInt32 Connect(TChar *pRemoteIp,TChar *pMyIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData) = 0;
    virtual TInt32 Connect(TUInt32 remoteIp,TUInt32 myIp,TUInt16 remotePort,TUInt16 myPort,void *pAppData) = 0;
    virtual TInt32 Run(TUInt32 runCnt) = 0;
    virtual TInt32 Disconnect(TUInt32 connectionIdx) = 0;
    virtual TInt32 SetAppData(TUInt32 connectionIdx,void *pAppData) = 0;
    virtual TInt32 Listen(TUInt32 ip,TUInt16 port,TUInt16 maxConnection,void *pAppData) = 0;
    virtual CConPair *GetConnectionInfo(TUInt32 connectionIdx) = 0 ;
};


}
#endif

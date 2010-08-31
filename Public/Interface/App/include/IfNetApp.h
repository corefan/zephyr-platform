/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfNet.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  传输层回调接口
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
#ifndef __IF_NET_APP_H__
#define __IF_NET_APP_H__

#include "SysMacros.h"
#include "TypeDef.h"

namespace Zephyr
{

class IfNetApp
{
public:
    virtual TInt32 OnRecv(TUInt32 connectionIdx,TUChar *pMsg, TUInt32 msgLen,void *pAppData) = 0;
    virtual TInt32 OnRecvIn2Piece(TUInt32 connectionIdx,TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
    virtual TInt32 OnConnected(TUInt32 connectIdx,void *pAppData) = 0;
    virtual TInt32 OnDissconneted(TUInt32 connectionIdx,void *pUserData) = 0;
    virtual TInt32 OnNetError(TUInt32 connectionIdx,void *pUserData) = 0;
};

}










#endif

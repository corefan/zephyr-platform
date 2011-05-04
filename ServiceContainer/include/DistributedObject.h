/*-------------------------------------------------------------
 Copyright (C)| 
 File: DistributedObject.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 一个分布式对象
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
#ifndef __ZEPHYR_SERVICE_CONTAINER_DISTRIBUTED_OBJECT_H__
#define __ZEPHYR_SERVICE_CONTAINER_DISTRIBUTED_OBJECT_H__

#include "../../Public/include/TypeDef.h"
#include "../../Public/include/SysMacros.h"
#include "../../Public/Interface/App/include/IfObj.h"
#include "../../Public/Interface/Platform/include/IfCommunicator.h"
#include "../../Public/Interface/Platform/include/IfSkeleton.h"
namespace Zephyr
{

class CObject : public IfObj
{
protected:
    IfSkeleton *m_pSkeleton;
    CMessageHeader *m_pCurrentMsg;
    //IfCommunicator  *m_pIfComm;

    TInt64      m_nMsgHandled;
    TInt64      m_nMsgSent;
    TInt64      m_nMsgDroped;
    TInt64      m_nStartTime;
public:
    CObject();
    TInt32 Init();
    TInt32      OnInit();
    /*virtual TInt32  OnRecv(CMessageHeader *pMsg); //这些函数如果*/

    //定时  器到时了
    virtual TInt32  OnTimer(TInt32 nTimerIdx,void *pData,TInt32 nTimeGap,TUInt32 nTimeNow)
    {
        return 0;
    }

    //以下是给Service使用的
    //定期的回调，可以注册循环时间，但只能有一个
    virtual TInt32  OnRoutine(TUInt32 nRunCnt)
    {
        return 0;
    }
    //网络时间
    virtual TInt32  OnNetEvent(CConnectionEvent *pEvent)
    {
        return SUCCESS;
    }

    CMessageHeader *GetCurrentMsg()
    {
        return m_pCurrentMsg;
    }
};

}



#endif

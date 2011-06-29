/*-------------------------------------------------------------
 Copyright (C)| 
 File: DistributedObject.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: һ���ֲ�ʽ����
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
    //ֻ����Service\ServiceContainerȥע��.
    void   SetSkeleton(IfSkeleton *pSkeleton)
    {
        m_pSkeleton = pSkeleton;
    }
    IfSkeleton *GetSkeleton()
    {
        return m_pSkeleton;
    }
    CObject();
    TInt32 Init();
    //�ڳ�ʼ����ʱ��ᱻ��.
    virtual TInt32      OnInit();
    //�����ǻص�.
    virtual TInt32      OnFinal();
    /*virtual TInt32  OnRecv(CMessageHeader *pMsg); //��Щ�������*/

    //��ʱ  ����ʱ��
    virtual TInt32  OnTimer(TInt32 nTimerIdx,void *pData,TInt32 nTimeGap,TUInt32 nTimeNow)
    {
        return 0;
    }

    //�����Ǹ�Serviceʹ�õ�
    //���ڵĻص�������ע��ѭ��ʱ�䣬��ֻ����һ��
    virtual TInt32  OnRoutine(TUInt32 nRunCnt)
    {
        return 0;
    }
    //����ʱ��
    virtual TInt32  OnNetEvent(CConnectionEvent *pEvent)
    {
        return SUCCESS;
    }

    const CMessageHeader *GetCurrentMsg()
    {
        return m_pCurrentMsg;
    } 
};

}



#endif

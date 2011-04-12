/*-------------------------------------------------------------
 Copyright (C)| 
 File: Orb.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: Orb�㣬ʹ��һ��IfComm
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
#ifndef __ZEPHYR_SERVICE_CONTAINER_ORB_H__
#define __ZEPHYR_SERVICE_CONTAINER_ORB_H__

#include "../../Public/include/TypeDef.h"
#include "../../Public/include/Clock.h"
#include "../../Public/Interface/Platform/include/IfOrb.h"
#include "../../Public/Interface/Platform/include/IfTask.h"
#include "../../Public/Interface/Platform/include/IfCommunicator.h"
#include "../../Public/tpl/include/TplPool.h"

#include "Skeleton.h"

#define MAX_SERVICE_NR 64

namespace Zephyr
{

class COrb : public IfOrb , public IfTask
{
private:
    IfCommunicator *m_pIfComm;
    IfObj          *m_pRunObj;
    TUInt32         m_nRunGapInMs;
    TUInt32         m_nLastCheckTime;
    TUInt16         m_nLocalNodeId;
    TUInt16         m_nLocalVIP;
    //���Orb��service�������ʼ
    TUInt16         m_nLocalServiceId;

    TUInt16         m_nServiceNr;
    //��16λ��ѭ��ʹ��objId
    TplArrayPool<CSkeleton>    m_tSkeletonPool;

    const CClock * m_pClock;
    CSkeleton       *m_ppServiceSkeleton[MAX_SERVICE_NR];

    //��Ҫrun�Ķ�������,40ms��һ��
    TUInt32         m_nLastRunTime;
    CList<CArrayPoolNode<CSkeleton> > m_tRunning;
             
public:
    COrb(IfCommunicator *pIfCom,CDoid *pDoidBegin,TInt32 nStubNr);
    virtual IfSkeleton* RegisterObj(IfObj *pObjSkeleton);
    //ע��
    virtual void    UnRegisterObj(IfSkeleton *pStub);

    //ʱ��ֻ����1ms��10ms��100ms��1s����
    virtual TInt32 RegisterRun(IfObj *pObj,TUInt32 nGapInMs);
    //ʱ�����
    virtual const CClock *GetClock();

public:
    virtual TInt32 Begin(TInt32 threadId)
    {
        return SUCCESS;
    }
    virtual TInt32 Run(const TInt32 threadId,const TInt32 runCnt);
    virtual TInt32 End(TInt32 threadId)
    {
        return SUCCESS;
    }
};

}

#endif

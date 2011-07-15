/*-------------------------------------------------------------
 Copyright (C)| 
 File: Skeleton.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: Orb层的Skeleton，代表一个Obj在Orb的注册结果
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SKELETON_H__
#define __ZEPHYR_SKELETON_H__

#include "../../include/TypeDef.h"
#include "../../Interface/Platform/include/IfSkeleton.h"
#include "../../Interface/App/include/IfObj.h"
#include "../../Interface/Platform/include/IfCommunicator.h"
namespace Zephyr
{

class COrb;

class CSkeleton : public IfSkeleton
{
private:
    DECLARE_CLASS_LIST(CSkeleton);
    
    IfObj           *m_pRegisteredObj;
    IfCommunicator  *m_pIfComm;
    CDoid           m_tDoid;
public:
    CSkeleton();
    virtual ~CSkeleton()
    {

    }
    void   OnReused(TUInt16 uStep);
    CDoid *GetMyDoid();
    //获得后自己初始化
    virtual CMessageHeader *PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid* destDoid,TInt32 destDoidNum,bool bRearrangeDest) ;
    //发送消息
    virtual TInt32  SendMsg(CMessageHeader *pMsg);

    TInt32    Init(IfCommunicator *pIfComm);
    
    TInt32    OnInit(); 

    void      OnFinal();

    TBOOL   IsMyMsg(CDoid *pDoid)
    {
        return (pDoid->m_objId == m_tDoid.m_objId);
    }
    TInt32  OnRecv(CMessageHeader *pMsg)
    {
        return m_pRegisteredObj->OnRecv(pMsg);
    }
};


}

#endif

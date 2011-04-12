/*-------------------------------------------------------------
 Copyright (C)| 
 File: Skeleton.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: Orb���Skeleton������һ��Obj��Orb��ע����
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_SKELETON_H__
#define __ZEPHYR_SKELETON_H__

#include "../../Public/include/TypeDef.h"
#include "../../Public/Interface/Platform/include/IfSkeleton.h"
#include "../../Public/Interface/App/include/IfObj.h"
#include "../../Public/Interface/Platform/include/IfCommunicator.h"
namespace Zephyr
{

class COrb;

class CSkeleton : public IfSkeleton
{
private:
    DECLARE_CLASS_LIST(CSkeleton);
    
    IfObj           *m_pRegisteredObj;
    IfCommunicator  *m_pIfComm;
    COrb            *m_pOrb; //ָ��Orb����ע��ʱ��
    CDoid           m_tDoid;
public:
    CSkeleton();
    void   OnReused(TUInt16 uStep);
    CDoid *GetMyDoid();
    //��ú��Լ���ʼ��
    virtual CMessageHeader *PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid* destDoid,TInt32 destDoidNum,bool bRearrangeDest) ;
    //������Ϣ
    virtual TInt32  SendMsg(CMessageHeader *pMsg);

    void    Init(IfCommunicator *pIfComm,COrb *pOrb,CDoid *pDoid);
    TBool   IsMyMsg(CDoid *pDoid)
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

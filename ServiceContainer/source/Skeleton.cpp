#include "../include/Skeleton.h"

namespace Zephyr
{

CSkeleton::CSkeleton()
{
    m_pRegisteredObj = NULL;
    m_pIfComm = NULL;
    m_pOrb  = NULL; //指向Orb，在注册时用
}
void CSkeleton::OnReused(TUInt16 uStep)
{
    m_tDoid.m_objId += uStep;
}
CDoid *CSkeleton::GetMyDoid()
{
    return &m_tDoid;
}
    //获得后自己初始化
CMessageHeader *CSkeleton::PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* destDoid,TInt32 destDoidNum,bool bRearrangeDest) 
{
   return m_pIfComm->PrepareMsg(bodyLength,methodId,srcId,destDoid,destDoidNum,bRearrangeDest);
}

    //发送消息
TInt32 CSkeleton::SendMsg(CMessageHeader *pMsg)
{
    return m_pIfComm->SendMsg(pMsg);
}


void CSkeleton::Init(IfCommunicator *pIfComm,COrb *pOrb,CDoid *pDoid)
{
    m_pRegisteredObj = NULL;
    m_pIfComm = pIfComm;
    m_pOrb = pOrb; //指向Orb，在注册时用
    m_tDoid = *pDoid;
}

}

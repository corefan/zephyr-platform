#include "../include/Skeleton.h"

namespace Zephyr
{

CSkeleton::CSkeleton()
{
    m_pRegisteredObj = NULL;
    m_pIfComm = NULL;
    m_pOrb  = NULL; //ָ��Orb����ע��ʱ��
}
void CSkeleton::OnReused(TUInt16 uStep)
{
    m_tDoid.m_objId += uStep;
}
CDoid *CSkeleton::GetMyDoid()
{
    return &m_tDoid;
}
    //��ú��Լ���ʼ��
CMessageHeader *CSkeleton::PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* destDoid,TInt32 destDoidNum,bool bRearrangeDest) 
{
   return m_pIfComm->PrepareMsg(bodyLength,methodId,srcId,destDoid,destDoidNum,bRearrangeDest);
}

    //������Ϣ
TInt32 CSkeleton::SendMsg(CMessageHeader *pMsg)
{
    return m_pIfComm->SendMsg(pMsg);
}


void CSkeleton::Init(IfCommunicator *pIfComm,COrb *pOrb,CDoid *pDoid)
{
    m_pRegisteredObj = NULL;
    m_pIfComm = pIfComm;
    m_pOrb = pOrb; //ָ��Orb����ע��ʱ��
    m_tDoid = *pDoid;
}

}

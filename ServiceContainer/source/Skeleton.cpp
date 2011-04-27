#include "../include/Skeleton.h"

namespace Zephyr
{

CSkeleton::CSkeleton()
{
    m_pRegisteredObj = NULL;
    m_pIfComm = NULL;
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
CMessageHeader *CSkeleton::PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid* destDoid,TInt32 destDoidNum,bool bRearrangeDest) 
{
   return m_pIfComm->PrepareMsg(bodyLength,methodId,m_tDoid,destDoid,destDoidNum,bRearrangeDest);
}

    //������Ϣ
TInt32 CSkeleton::SendMsg(CMessageHeader *pMsg)
{
    return m_pIfComm->SendMsg(pMsg);
}


TInt32 CSkeleton::Init(IfCommunicator *pIfComm)
{
    m_pRegisteredObj = NULL;
    m_pIfComm = pIfComm;
    return SUCCESS;
}


}

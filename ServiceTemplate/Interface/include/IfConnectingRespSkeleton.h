#ifndef __IFCONNECTINGRESP_SKELETON_H__
#define __IFCONNECTINGRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/ifconnectingresp.h"
#include "IfConnectingRespMethodId.h"
namespace Zephyr 
{
class IfConnectingRespSkeleton 
{
public:
    IfConnectingResp *m_pImplementObj;
    IfConnectingRespSkeleton(IfConnectingResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //�Ƿ���������ӿ�
    {
         return ((IfConnectingRespServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleRespRegisterService_CDoid_pt_TUInt32_TInt32(CMessageHeader *pMsg);
    TInt32 HandleConfirmUnregisterService_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleOnDisconnect_TUInt32(CMessageHeader *pMsg);
};
}
#endif

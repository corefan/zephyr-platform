#ifndef __IFCONNECTING_SKELETON_H__
#define __IFCONNECTING_SKELETON_H__
namespace Zephyr 
{
class IfConnectingSkeleton 
{
public:
    IfConnecting *m_pImplementObj;
    IfConnectingSkeleton(IfConnecting *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBool  IsMine(CMessageHeader *pMsg); //是否属于这个接口
    TInt32 HandleRegisterService_TUInt32_TUInt32_CDoid_pt(CMessageHeader *pMsg);;
    TInt32 HandleUnregisterService_TUInt32_TUInt32_CDoid_pt(CMessageHeader *pMsg);;
    TInt32 HandleDisconnect_TUInt32(CMessageHeader *pMsg);;
    TInt32 HandleSetId_TUInt32(CMessageHeader *pMsg);;
    TInt32 HandleCheckId_TUInt32(CMessageHeader *pMsg);;
};
}
#endif

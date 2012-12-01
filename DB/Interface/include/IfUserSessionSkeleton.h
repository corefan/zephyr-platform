#ifndef __IFUSERSESSION_SKELETON_H__
#define __IFUSERSESSION_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfUserSession.h"
#include "IfUserSessionMethodId.h"
namespace erp_platform 
{
class IfUserSessionSkeleton 
{
public:
    IfUserSession *m_pImplementObj;
    IfUserSessionSkeleton(IfUserSession *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfUserSessionServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleChangePwd_TChar_pt_TChar_pt(CMessageHeader *pMsg);
    TInt32 HandleReqGetMyInfo_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleReqGetUserInfo_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleReqGetDepartmentInfo_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleReqGetMenuInfoList_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleReqUpdateMenuInfo_TJobInfos_ref(CMessageHeader *pMsg);
    TInt32 HandleReqGetActiveWorkInfo_TInt32_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleReqGetMyWorkList_TUInt32_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleReqGetMenuInfoById_TUInt32(CMessageHeader *pMsg);
};
}
#endif

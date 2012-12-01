#ifndef __IFUSERSESSIONRESP_SKELETON_H__
#define __IFUSERSESSIONRESP_SKELETON_H__
#include "Public/include/Message.h"
#include "../Interface/IfUserSessionResp.h"
#include "IfUsersessionRespMethodId.h"
namespace erp_platform 
{
class IfUsersessionRespSkeleton 
{
public:
    IfUsersessionResp *m_pImplementObj;
    IfUsersessionRespSkeleton(IfUsersessionResp *pIfObj)
    {
        m_pImplementObj = pIfObj;
    }
    TInt32 HandleMsg(CMessageHeader *pMsg);
    static TBOOL  IsMine(CMessageHeader *pMsg) //是否属于这个接口
    {
         return ((IfUsersessionRespServiceIdBegin)==(pMsg->GetServiceInterfaceId()));
    }
    TInt32 HandleOnPwdChanged_TInt32(CMessageHeader *pMsg);
    TInt32 HandleRespGetMyInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleRespGetUserInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleRespGetDepartInfo_TUInt32_TChar_pt_TUInt32(CMessageHeader *pMsg);
    TInt32 HandleSendJobList_list_tpl_begin_TJobInfos_tpl_end__ref_list_tpl_begin_TJobInfos_tpl_end__ref(CMessageHeader *pMsg);
    TInt32 HandleSendActiveWork_TJobInfos_TUInt32_const_TChar_pt(CMessageHeader *pMsg);
    TInt32 HandleRespMenuList_list_tpl_begin_TJobInfos_tpl_end_(CMessageHeader *pMsg);
    TInt32 HandleRespGetMenuInfo_TJobInfos_ref(CMessageHeader *pMsg);
};
}
#endif

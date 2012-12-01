#ifndef __IFUSERSESSIONRESP_STUB_H__
#define __IFUSERSESSIONRESP_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfUserSessionResp.h"
namespace erp_platform 
{
class IfUsersessionRespStub : public IfUsersessionResp
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 OnPwdChanged(TInt32 nResult);
    virtual TInt32 RespGetMyInfo(TUInt32 uUpdateTime,TChar* pszName,TChar* pszApartName,TUInt32 uMyId,TUInt32 uApartId);
    virtual TInt32 RespGetUserInfo(TUInt32 uUpdateTime,TChar* pszName,TChar* pszApartName,TUInt32 uMyId,TUInt32 uApartId);
    virtual TInt32 RespGetDepartInfo(TUInt32 uUpdateTime,TChar* pszApartName,TUInt32 uApartId);
    virtual TInt32 SendJobList(list<TJobInfos>& tMenu2,list<TJobInfos>& tMeun3);
    virtual TInt32 SendActiveWork(TJobInfos tJobs,TUInt32 uRcdId,const TChar* pszTitle);
    virtual TInt32 RespMenuList(list<TJobInfos> tList);
    virtual TInt32 RespGetMenuInfo(TJobInfos& tJobInfo);
};
}
#endif

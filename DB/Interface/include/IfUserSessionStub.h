#ifndef __IFUSERSESSION_STUB_H__
#define __IFUSERSESSION_STUB_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "../Interface/IfUserSession.h"
namespace erp_platform 
{
class IfUserSessionStub : public IfUserSession
{
public:
    IfSkeleton *m_pOnwerObj;
    CDoid  m_tTarget;
    void Init(IfSkeleton *pSkeleton,const CDoid *pDoid)
    {
        m_pOnwerObj = pSkeleton;
        m_tTarget = *pDoid;
    }
    virtual TInt32 ChangePwd(TChar* pOldPwd,TChar* pszNewPwd);
    virtual TInt32 ReqGetMyInfo(TUInt32 uLastUpdateTime);
    virtual TInt32 ReqGetUserInfo(TUInt32 uUserId);
    virtual TInt32 ReqGetDepartmentInfo(TUInt32 uDepartmentId);
    virtual TInt32 ReqGetMenuInfoList(TUInt32 uFatherId);
    virtual TInt32 ReqUpdateMenuInfo(TJobInfos& rJobInfos);
    virtual TInt32 ReqGetActiveWorkInfo(TInt32 nJobId,TUInt32 uRcdId);
    virtual TInt32 ReqGetMyWorkList(TUInt32 uFromTime,TUInt32 uToTime);
    virtual TInt32 ReqGetMenuInfoById(TUInt32 uMenuId);
};
}
#endif

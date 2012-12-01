#ifndef __ZEPHYR_IF_USER_SESSION_RESP_H__
#define __ZEPHYR_IF_USER_SESSION_RESP_H__

#include "Public/include/TypeDef.h"
#include <map>
#include <list>
#include "JobInfo.h"
using namespace std;
using namespace Zephyr;

namespace erp_platform
{
    
// 
//     struct TJobList
//     {
//         list<TJobInfos> m_tJobLists;
//     };

class IfUsersessionResp
{
public:
    virtual TInt32 OnPwdChanged(TInt32 nResult) = 0;
    //UpdateTime�������ʱ��,pszName����,pszApartName,��������,uMyId,�ҵ�����ID,uApartId,����ID
    virtual TInt32 RespGetMyInfo(TUInt32 uUpdateTime,TChar *pszName,TChar *pszApartName,TUInt32 uMyId,TUInt32 uApartId) = 0;
    //���ر��˵���Ϣ,�������Ӧ���Ǿ������õ�.
    virtual TInt32 RespGetUserInfo(TUInt32 uUpdateTime,TChar *pszName,TChar *pszApartName,TUInt32 uMyId,TUInt32 uApartId) = 0;
    //���ز�����Ϣ
    virtual TInt32 RespGetDepartInfo(TUInt32 uUpdateTime,TChar *pszApartName,TUInt32 uApartId) = 0;

    //���ص��ǹ����б����ֻ�е�¼��ʱ��ᷢ��һ�Σ�����
    virtual TInt32 SendJobList(list<TJobInfos> &tMenu2,list<TJobInfos> &tMeun3) = 0;

    //���͵�ǰ����,tJobs�������Ĺ������̣�uRcdId,�����������Ӧ�����ݿ��¼��ID��pszTitle�����⣬����ᷢ��Σ����ң���ʵʱ������
    virtual TInt32 SendActiveWork(TJobInfos tJobs,TUInt32 uRcdId,const TChar *pszTitle) = 0;
    
    //���ǵ�fatherId������ͬ�ģ��������id
    virtual TInt32 RespMenuList(list<TJobInfos> tList) = 0;

    //��ȡ�ض���������
    virtual TInt32 RespGetMenuInfo(TJobInfos &tJobInfo) = 0;
};

}

#endif

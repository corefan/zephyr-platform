#ifndef __ZEPHYR_DB_AUTHENTICATE_SERVICE_H__
#define __ZEPHYR_DB_AUTHENTICATE_SERVICE_H__


#include "Public/include/SysMacros.h"
#include "Public/Orb/include/Service.h"
#include "../../Interface/Interface/IfAuthService.h"
#include "Public/include/IdlHeaderFile.h"
#include "Public/tpl/include/TplPool.h"
#include "Public/tpl/include/TplList.h"
#include "Public/include/TypeDef.h"
#include "AuthenticateServiceLogger.h"
#include "System/DBLib/IfDBLib.h"
#include "DBAuthenticateTrans.h"
#include "Public/tpl/include/tplmap.h"


namespace Zephyr
{

using namespace DBLib;

class CAuthenticateService : public CService,
                             public IfAuthService
{
private:
    TplMap<CDBAuthenticateTrans,CDoid> m_tUsingMaps;
    CPool<TplNode<CDBAuthenticateTrans,CDoid> > m_tTransPool;

    //再数据库验证完毕之后，放入这个队列，只有等全部的后台session创建完毕后，再把他删掉,.
    //std::list<CDBAuthenticateTrans*> m_tInitingSessionLists;
    
    //TplMap<>
    IfTrascationWorkThreadMgr  *m_pDbMgr;
    IfOrb *m_pIfOrb;
    IfTaskMgr *m_pIfTaskMgr;
    IfLoggerManager *m_pLoggerMgr;
    IfLogger        *m_pLogger;
    TInt32          m_nMaxTransNum;
    TInt32          m_nPendingDBTrans;
    TUInt64         m_nTotalReqTrans;
    TUInt64         m_nTotalRetTrans;
    //以上两个相减就是还在处理中的
    TUInt32         m_nLastStaticTime;
    //统计信息,1分钟统计一次
    TUInt32         m_nTotalReqTransIn1Min;
public:
    CAuthenticateService();
    ~CAuthenticateService();
    virtual TInt32      OnInit();
    //结束是回调.
    virtual void      OnFinal();
    virtual TInt32  OnRoutine(TUInt32 nRunCnt);

    TInt32 InitService(IfOrb *pIfOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pLoggerMgr);
    DECALRE_HANDLE_INTERFCE;
    //在初始化的时候会被调.
    
    
    virtual TInt32 Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData); 
    virtual TInt32 OnDisconneted(CDoid tMyDoid);
    TInt32 StartService(TChar *pszLoggerName,TChar *pszConnectStr,TInt32  nThreadCount,TInt32 QueueSize,TUInt32 Flag=0);

    IfLogger *GetLogger()
    {
        return m_pLogger;
    }
    void OnDbFinished(CDBAuthenticateTrans *pTrans);
};



#ifdef WIN32

extern "C" __declspec( dllexport ) CService *InitService(IfOrb* pOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr);

extern "C" __declspec( dllexport ) TInt32 ReleaseService(CService *);

#else

extern "C" CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr);
extern "C" TInt32 ReleaseService(CService *);

#endif


}

#endif

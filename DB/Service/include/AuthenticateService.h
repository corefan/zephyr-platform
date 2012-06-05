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

    //�����ݿ���֤���֮�󣬷���������У�ֻ�е�ȫ���ĺ�̨session������Ϻ��ٰ���ɾ��,.
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
    //��������������ǻ��ڴ����е�
    TUInt32         m_nLastStaticTime;
    //ͳ����Ϣ,1����ͳ��һ��
    TUInt32         m_nTotalReqTransIn1Min;
public:
    CAuthenticateService();
    ~CAuthenticateService();
    virtual TInt32      OnInit();
    //�����ǻص�.
    virtual void      OnFinal();
    virtual TInt32  OnRoutine(TUInt32 nRunCnt);

    TInt32 InitService(IfOrb *pIfOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pLoggerMgr);
    DECALRE_HANDLE_INTERFCE;
    //�ڳ�ʼ����ʱ��ᱻ��.
    
    
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

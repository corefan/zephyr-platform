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



namespace Zephyr
{

using namespace DBLib;

class CAuthenticateService : public CService,
                             public IfAuthService
{
private:
    IfTrascationWorkThreadMgr  *m_pDbMgr;
    IfOrb *m_pIfOrb;
    IfTaskMgr *m_pIfTaskMgr;
    IfLoggerManager *m_pLoggerMgr;
    IfLogger        *m_pLogger;
public:
    //�ڳ�ʼ����ʱ��ᱻ��.
    virtual TInt32      OnInit();
    //�����ǻص�.
    virtual TInt32      OnFinal();
    
    TInt32 InitService(IfOrb *pIfOrb,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pLoggerMgr);
    CAuthenticateService();
    ~CAuthenticateService();
    virtual TInt32 Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData); 

    TInt32 OnRecv(Zephyr::CMessageHeader *)
    {
        return 0;
    }

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

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
public:
    CAuthenticateService();
    ~CAuthenticateService();
    virtual TInt32 Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData); 
};
}


#endif

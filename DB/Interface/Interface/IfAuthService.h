#ifndef __ZEPHYR_DB_IF_AUTH_SERVICE_H__
#define __ZEPHYR_DB_IF_AUTH_SERVICE_H__

#include "Public/include/TypeDef.h"

namespace erp_platform
{

class IfAuthService
{
public:
    virtual TInt32 Authenticate(TUInt32 uDyncNr,TChar *pszName,TChar *pszPwd) = 0;
    virtual TInt32 ChangePwd(TChar *pszName,TChar *pszOldPwd,TChar *pNewPwd) = 0;
    virtual TInt32 OnDisconneted(CDoid tMyDoid) = 0;
};

}

#endif

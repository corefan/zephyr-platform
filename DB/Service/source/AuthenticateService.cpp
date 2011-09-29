#include "../include/AuthenticateService.h"

namespace Zephyr
{
CAuthenticateService::CAuthenticateService()
{

}

CAuthenticateService::~CAuthenticateService()
{

}

TInt32 CAuthenticateService::Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData)
{
    return SUCCESS;
}

}


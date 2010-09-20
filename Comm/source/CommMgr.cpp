#include "CommMgr.h"
#include "SettingFile.h"
namespace Zephyr
{


TInt32 CCommMgr::Init(const TChar *pConfigName/* =szDefaultLoggerName */)
{
    return SUCCESS;
}

IfCommunicator *CCommMgr::RegisterWorker(TUInt16 srvId)
{
    return NULL;
}

}
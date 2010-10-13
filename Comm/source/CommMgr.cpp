#include "CommMgr.h"
#include "SettingFile.h"
namespace Zephyr
{


TInt32 CCommMgr::Init(const TChar *pConfigName/* =szDefaultLoggerName */)
{
    TInt32 ret = m_ipMaps.Init(pConfigName);
    if (SUCCESS > ret)
    {
        return ret;
    }
	
    for (int i = 0;i<m_ipMaps.m_nrOfVirtualIp;++i)
    {
        if (i != m_ipMaps.m_localVirtualIp)
        {
            
        }
    }
    
    return SUCCESS;
}

IfCommunicator *CCommMgr::RegisterWorker(TUInt16 srvId)
{
    return NULL;
}

}
#include "IpMaps.h"
#ifdef _WIN32
#include "winsock2.h"
#endif
#include "stdio.h"
#include "SettingFile.h"
#include "SysMacros.h"
namespace Zephyr
{

CIpMap::CIpMap()
{
    m_nrOfNodes = 0;
    m_nrOfVirtualIp = 0;
    m_localNodeId = 0;
    m_localVirtualIp = 0;
    m_pVirtualIps = NULL;
}

TInt32 CIpMap::Init(const TChar *pConfigName)
{
    CSettingFile settingFile;
    if (!settingFile.Load(pConfigName))
    {
        return NULL_POINTER;
    }
    m_nrOfNodes = settingFile.GetInteger("MAIN","nrOfNodes");
    m_nrOfVirtualIp = settingFile.GetInteger("MAIN","nrOfVirtualIp");
    m_localNodeId    = settingFile.GetInteger("MAIN","localNodeId");
    m_localVirtualIp = settingFile.GetInteger("MAIN","localVirtualIp");
    
    if (m_nrOfNodes > 0)
    {
        NEW(m_pVirtualIps,TVirtualIp,(m_nrOfVirtualIp+1));
        if (!m_pVirtualIps)
        {
            return OUT_OF_MEM;
        }
    }
    for (int i=0;i<m_localVirtualIp;++i)
    {
        char buff[64];
        sprintf(buff,"VIP%d",i);
        const char* pIp = settingFile.GetString(buff,"ip");
        if (pIp)
        {
            m_pVirtualIps[i].m_realIp = inet_addr(pIp);
        }
        else
        {
            return NULL_POINTER;
        }
        m_pVirtualIps[i].m_bindPort = settingFile.GetInteger(buff,"bindPort",0);
        m_pVirtualIps[i].m_listenPort = settingFile.GetInteger(buff,"listenPort",0);
        if (0 == m_pVirtualIps[i].m_listenPort)
        {
            return NULL_POINTER;
        }
    }
    return SUCCESS;
}

}

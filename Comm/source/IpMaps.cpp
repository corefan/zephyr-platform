#include "IpMaps.h"

namespace Zephyr
{

CIpMap::CIpMap()
{
    m_nrOfNodes = 0;
    m_nrOfVirtualIp = 0;
    m_localNodeId = 0;
    m_localVirtualIp = 0;
    
}

void CIpMap::Init(const TChar *pConfigName)
{
    CSettingFile settingFile;
    settingFile.Load(pConfigName);
    m_nrOfNodes = settingFile.GetInteger("MAIN","nrOfNodes");
    m_nrOfVirtualIp = settingFile.GetInteger("MAIN","nrOfVirtualIp");
    m_localNodeId    = settingFile.GetInteger("MAIN","localNodeId");
    m_localVirtualIp = settingFile.GetInteger("MAIN","localVirtualIp");

}

}

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

TInt32 CIpMap::Init(const TChar *pConfigName,IfConnection *pSelf)
{
    CSettingFile settingFile;
    if (!settingFile.LoadFromFile(pConfigName))
    {
        return NULL_POINTER;
    }
    m_nrOfNodes = settingFile.GetInteger("MAIN","nrOfNodes");
    m_nrOfVirtualIp = settingFile.GetInteger("MAIN","nrOfVirtualIp");
    m_localNodeId    = settingFile.GetInteger("MAIN","localNodeId");
    m_localVirtualIp = settingFile.GetInteger("MAIN","localVirtualIp");
    
    if (m_nrOfNodes > 0)
    {
        NEW(m_pVirtualIps,CIpMapItem,(m_nrOfVirtualIp+1));
        if (!m_pVirtualIps)
        {
            return OUT_OF_MEM;
        }
    }
    for (int i=0;i<m_nrOfVirtualIp;++i)
    {
        char buff[64];
        sprintf(buff,"VIP%d",i);
        const char* pIp = settingFile.GetString(buff,"ip");
        if (pIp)
        {
            m_pVirtualIps[i].m_tKey.m_realIp = inet_addr(pIp);
        }
        else
        {
            return NULL_POINTER;
        }
        m_pVirtualIps[i].m_tKey.m_bindPort = settingFile.GetInteger(buff,"bindPort",0);
        m_pVirtualIps[i].m_tKey.m_listenPort = settingFile.GetInteger(buff,"listenPort",0);
        if (0 == m_pVirtualIps[i].m_tKey.m_listenPort)
        {
            return NULL_POINTER;
        }
    }
    if (m_nrOfNodes > 1)
    {
        m_pRoutes = new TUInt32[m_nrOfNodes];
        for (int i=0;i<m_nrOfNodes;++i)
        {
            char buff[64];
            sprintf(buff,"NODE%d",i);
            m_pRoutes[i] = settingFile.GetInteger(buff,"nodeGatewayIp");
            if (m_pRoutes[i] == m_localVirtualIp)
            {
                m_redirectIdx = m_nrOfVirtualIp;
                const char *pIp = settingFile.GetString(buff,"ip");
                if (pIp)
                {
                    m_connectedNodeInfo.m_realIp = atoi(pIp);
                }
                else
                {
                    return NULL_POINTER;
                }
                m_connectedNodeInfo.m_bindPort = settingFile.GetInteger(buff,"bindPort",0);
                m_connectedNodeInfo.m_listenPort = settingFile.GetInteger(buff,"listenPort",0);
            }
        }
    }
    else
    {
        m_pRoutes = NULL;
        m_redirectIdx = -1;
        m_connectedNodeInfo.m_key = 0xFFFFFFFFFFFFFFFF;
    }
    m_pVirtualIps[m_localNodeId].m_pIfConnection = pSelf;
    return SUCCESS;
}

}

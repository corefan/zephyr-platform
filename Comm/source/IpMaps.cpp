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
    m_pRoutes = NULL;
    m_pListening = NULL;
    m_nNrOfMapItem = 0;
}

int CIpMap::ReadIpMapItem(void *pFile,char *pMain,CIpMapItem *pItem)
{
    CSettingFile &file = *((CSettingFile*)pFile);
    const char* pIp = file.GetString(pMain,"myIp");
    unsigned int myIp = 0;
    if (pIp)
    {
        myIp = inet_addr(pIp);
    }
    else
    {
        return -1;
    }
    unsigned int remoteIp = 0;
    pIp = file.GetString(pMain,"remoteIp");
    if (pIp)
    {
        remoteIp = inet_addr(pIp);
    }
    else
    {
        return -1;
    }
    unsigned short myPort = 0;
    unsigned short remotePort = 0;
    myPort = file.GetInteger(pMain,"myPort");
    remotePort = file.GetInteger(pMain,"remotePort");
    if (!(remotePort && myPort))
    {
        return -1;
    }
    pItem->m_tKey.Init(remoteIp,myIp,remotePort,myPort);
    return SUCCESS;
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

    m_nNrOfMapItem = m_nrOfVirtualIp;
    if (m_nrOfNodes > 1)
    {
        for (int i=0;i<m_nrOfNodes;++i)
        {
            if (i == m_localNodeId)
            {
                continue;
            }
            char buff[64];
            sprintf(buff,"NODE%d",i);
            if (m_localVirtualIp == settingFile.GetInteger(buff,"localVIP"))
            {
                ++m_nNrOfMapItem;
            }
        }
    }
    //if (m_nrOfNodes > 0)
    {
        NEW(m_pVirtualIps,CIpMapItem,(m_nNrOfMapItem));
        if (!m_pVirtualIps)
        {
            return OUT_OF_MEM;
        }
    }
    m_pListening = new TUInt16[m_nNrOfMapItem];
    m_nNrOfLisenting = 0;
    char buff[64];
    for (int i=0;i<m_nrOfVirtualIp;++i)
    {
        sprintf(buff,"VIP%d",i);
        if (ReadIpMapItem(&settingFile,buff,&m_pVirtualIps[i]) < 0)
        {
            return -1;
        }
        m_pVirtualIps[i].m_nNodeId = m_localNodeId;
        m_pVirtualIps[i].m_nVirtualIp = i;
        //if (i)
        if (!IsPostive(i))
        {
            if (FALSE == IsListeningExisted(&m_pVirtualIps[i],(i)))
            {
                m_pListening[m_nNrOfLisenting] = i;
                ++m_nNrOfLisenting;
            }
        }
    }
    int usedNode = 0;

    //获取我的node信息
    if (m_nrOfNodes > 1)
    {
//         sprintf(buff,"NODE%d",m_localNodeId);
// 
//         //if (m_localVirtualIp == settingFile.GetInteger(buff,"localVIP"))
//         unsigned int realIp = 0;
//         unsigned short myPort = 0;
//         unsigned short remotePort = 0;
//         {
//             
//             const char* pIp = settingFile.GetString(buff,"nodeGatewayIp");
//             unsigned int realIp;
//             if (pIp)
//             {
//                 realIp = inet_addr(pIp);
//             }
//             else
//             {
//                 return NULL_POINTER;
//             }
//             bindPort = settingFile.GetInteger(buff,"bindPort",0);
//         }

        m_pRoutes = new TUInt32[m_nrOfNodes];
        memset(m_pRoutes,0,(sizeof(TUInt32)*m_nrOfNodes));
        m_pRoutes[m_localNodeId] = m_localVirtualIp; 
        for (int i=0;i<m_nrOfNodes;++i)
        {
            if (i == m_localNodeId)
            {
                m_pRoutes[i] = 0;
                continue;
            }
            sprintf(buff,"NODE%d",i);

            if (m_localVirtualIp == settingFile.GetInteger(buff,"localVIP"))
            {
                m_pRoutes[i] = m_nrOfVirtualIp + usedNode;
                if (ReadIpMapItem(&settingFile,buff,&m_pVirtualIps[m_pRoutes[i]]) < 0)
                {
                    return -1;
                }
                m_pVirtualIps[m_pRoutes[i]].m_nNodeId = i;
                m_pVirtualIps[m_pRoutes[i]].m_nVirtualIp = 0;
                if (FALSE == IsListeningExisted(&m_pVirtualIps[m_pRoutes[i]],(m_pRoutes[i])))
                {
                    m_pListening[m_nNrOfLisenting] = m_pRoutes[i];
                    ++m_nNrOfLisenting;
                }
//                 const char* pIp = settingFile.GetString(buff,"remoteIp");
//                 if (pIp)
//                 -1
//                     m_pVirtualIps[m_pRoutes[i]].m_tKey.m_remoteIp = inet_addr(pIp);
//                 }
//                 else
//                 {
//                     return NULL_POINTER;
//                 }
//                 pIp = settingFile.GetString(buff,"myIp");
//                 if (pIp)
//                 {
//                     m_pVirtualIps[m_pRoutes[i]].m_tKey.m_myIp = inet_addr(pIp);
//                 }
//                 else
//                 {
//                     return NULL_POINTER;
//                 }
// 
//                 if (i < m_localNodeId)
//                 {
//                     m_pVirtualIps[m_pRoutes[i]].m_tKey.m_nMyPort = bindPort + i;
//                 }
//                 else
//                 {
//                     m_pVirtualIps[m_pRoutes[i]].m_tKey.m_bindPort = settingFile.GetInteger(buff,"bindPort",0) + m_localNodeId;
//                 }
//                 m_pVirtualIps[m_pRoutes[i]].m_tKey.m_listenPort = settingFile.GetInteger(buff,"listenPort",0);
//                 if (0 == m_pVirtualIps[m_pRoutes[i]].m_tKey.m_listenPort)
//                 {
//                     return NULL_POINTER;
//                 }
//                 m_pVirtualIps[m_pRoutes[i]].m_nNodeId = i;
//                 m_pVirtualIps[m_pRoutes[i]].m_nVirtualIp = settingFile.GetInteger(buff,"remoteVIP",0);
                //m_pVirtualIps[m_pRoutes[i]]
                ++usedNode;
            }
            else
            {
                m_pRoutes[i] = settingFile.GetInteger(buff,"localVIP");
            }
        }
    }
    //不try了
    TUInt16 *pN = new TUInt16[m_nNrOfLisenting];
    memcpy(pN,m_pListening,sizeof(TUInt16)*m_nNrOfLisenting);
    delete [] m_pListening;
    m_pListening = pN;
    
//     if (m_nrOfNodes > 1)
//     {
//         m_pRoutes = new TUInt32[m_nrOfNodes];
//         for (int i=0;i<m_nrOfNodes;++i)
//         {
//             char buff[64];
//             sprintf(buff,"NODE%d",i);
//             m_pRoutes[i] = settingFile.GetInteger(buff,"nodeGatewayIp");
//             if (m_pRoutes[i] == m_localVirtualIp)
//             {
//                 m_redirectIdx = m_nrOfVirtualIp;
//                 const char *pIp = settingFile.GetString(buff,"ip");
//                 if (pIp)
//                 {
//                     m_connectedNodeInfo.m_realIp = atoi(pIp);
//                 }
//                 else
//                 {
//                     return NULL_POINTER;
//                 }
//                 m_connectedNodeInfo.m_bindPort = settingFile.GetInteger(buff,"bindPort",0);
//                 m_connectedNodeInfo.m_listenPort = settingFile.GetInteger(buff,"listenPort",0);
//             }
//         }
//     }
//     else
//     {
//         m_pRoutes = NULL;
//         m_redirectIdx = -1;
//         m_connectedNodeInfo.m_key = 0xFFFFFFFFFFFFFFFF;
//     }
    m_pVirtualIps[m_localNodeId].m_pIfConnection = pSelf;
    return SUCCESS;
}

}

/*-------------------------------------------------------------
 Copyright (C)| 
 File: CommConnection.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  �����IP���ñ���ע������ip = 0 ������������,�ڲ�ͬ�ڵ�Ļ���������·��.
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_COMM_IP_MAPS_H__
#define __ZEPHYR_COMM_IP_MAPS_H__
#include "Typedef.h"
#include "CommConnection.h"
#include "Doid.h"
namespace Zephyr
{

struct TVirtualIp
{
    TUInt32 m_realIp;
    TUInt16 m_bindPort;
    TUInt16 m_listenPort;
};

class CIpMap
{
private:
    TUInt16              m_nrOfNodes;
    TUInt16              m_nrOfVirtualIp;
    TUInt16              m_localNodeId;
    TUInt16              m_localVirtualIp;
    TVirtualIp           *m_pVirtualIps;
    //CCommConnection      *m_pLocalConnections;
public:
    CIpMap();
    TInt32 Init(const TChar *pConfigName);
    TBool IsLocal(CDoid *pDoId)
    {
        if((pDoId->m_nodeId == m_localNodeId) && (pDoId->m_virtualIp == m_localVirtualIp))
        {
            return TRUE;
        }
        return FALSE;
    }
//     CCommConnection *GetConnection(CDoid *pDoid)
//     {
//         //ֻ���nodeId,��srvId
//         if((pDoid->m_nodeId == m_localNodeId))
//         {
//             if ((pDoid->m_virtualIp != m_localVirtualIp))
//             {
//                 if (pDoid->m_virtualIp < m_nrOfVirtualIp)
//                 {
//                     //return (m_pLocalConnections + pDoid->m_virtualIp);
//                 }
//             }
//         }
//         else
//         {
//             if (pDoid->m_nodeId < m_nrOfNodes)
//             {
//                 return m_pLocalConnections;
//             }
//         }
//         return NULL;
//     }
};
}
#endif
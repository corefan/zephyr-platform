/*-------------------------------------------------------------
 Copyright (C)| 
 File: CommConnection.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  �����IP���ñ�ע������ip = 0 ������������,�ڲ�ͬ�ڵ�Ļ���������·��.
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

union TVirtualIp
{
    struct
    {
        TUInt32     m_realIp;
        TUInt16     m_bindPort;
        TUInt16     m_listenPort;
        //TUInt32     m_srvType;
    };
    TUInt64 m_key;
};

class CIpMapItem
{
public:
    TVirtualIp      m_tKey;
    TUInt32         m_uLastConnectTime;
    CCommConnection *m_pConnection;
};

class CIpMap
{
public:
    //��д
    TUInt16              m_nrOfNodes;
    TUInt16              m_nrOfVirtualIp;
    TUInt16              m_localNodeId;
    TUInt16              m_localVirtualIp;
    CIpMapItem           *m_pVirtualIps;
    //ת��·�ɱ�,����ͬ��node.
    TUInt32              *m_pRoutes;


 

    //�����ƽ̨�����ӵ������ڵ����Ϣ
    TInt32               m_connectedNode;
    TUInt32              m_redirectIdx;
    TVirtualIp           m_connectedNodeInfo;
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

    CCommConnection *RouteTo(CDoid *pDoId)
    {
        if (pDoId->m_nodeId < m_nrOfNodes)
        {
            if (pDoId->m_nodeId == m_connectedNode)
            {
                if (pDoId->m_virtualIp < m_nrOfVirtualIp)
                {
                    return m_pVirtualIps[pDoId->m_virtualIp].m_pConnection;
                }
            }
            else
            {
                return m_pVirtualIps[m_pRoutes[pDoId->m_nodeId]].m_pConnection;
            }
            //return m_pRoutes[pDoId->m_nodeId];
        }
        //ֱ�Ӷ���.
        return NULL;
    }
};

}
#endif

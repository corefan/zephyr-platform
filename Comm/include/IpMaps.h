/*-------------------------------------------------------------
 Copyright (C)| 
 File: CommConnection.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  传输层IP配置表，注意虚拟ip = 0 被用来在网关,在不同节点的机器走网关路由.
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
    CIpMapItem()
    {
        m_tKey.m_key = 0;
        m_uLastUsedTime = 0;
        m_pConnection = 0;
    }
    //这个是一开始就初始化好的

    TVirtualIp          m_tKey;
    TUInt32             m_uLastUsedTime;
    CCommConnection    *m_pConnection;
    IfConnection       *m_pIfConnection;
    void OnConnecting(CCommConnection *pCon,TUInt32 uTimeNow)
    {
        m_pConnection = pCon;
        m_uLastUsedTime = uTimeNow;
    }
    //被动连接的调用这个，返回上一个CCommConnection
    CCommConnection *OnConnected(CCommConnection *pConnection,TUInt32 uTimeNow)
    {
        CCommConnection *pOld = m_pConnection;
        m_pConnection = pConnection;
        m_pIfConnection = pConnection->GetIfConnection();
        m_uLastUsedTime = uTimeNow;
        return pOld;
    }
    void OnDisconnected(TUInt32 uTimeNow)
    {
        m_pConnection = NULL;
        m_pIfConnection = NULL;
        m_uLastUsedTime = uTimeNow;
    }
    void OnUsed(TUInt32 uUseTime)
    {
        m_uLastUsedTime = uUseTime; 
    }
    TUInt32 GetLastUsedTime()
    {
        return m_uLastUsedTime;
    }
};

class CIpMap
{
public:
    //不写
    TUInt16              m_nrOfNodes;
    TUInt16              m_nrOfVirtualIp;
    TUInt16              m_localNodeId;
    TUInt16              m_localVirtualIp;
    CIpMapItem           *m_pVirtualIps;
    //转发路由表,发向不同的node.
    TUInt32              *m_pRoutes;


 

    //保存该平台所连接的其他节点的信息
    TInt32               m_connectedNode;
    TUInt32              m_redirectIdx;
    TVirtualIp           m_connectedNodeInfo;

    
    //CCommConnection      *m_pLocalConnections;
public:
    CIpMap();

    //只有被动连接调用这个，
    TInt32 GetIpMapInfo(TUInt16 &nNodeId,TUInt16 &nVip,CConPair *pConn)
    {
        TVirtualIp conn;
        conn.m_realIp = pConn->GetRemoteIp();
        conn.m_bindPort = pConn->GetRemotePort();
        conn.m_listenPort = pConn->GetMyPort();
        for (int i = 0;i<m_nrOfVirtualIp;++i)
        {
            if (conn.m_key == m_pVirtualIps[i].m_tKey.m_key)
            {
                nNodeId = m_localNodeId;
                nVip    = i;
                return SUCCESS;
            }
        }
        //走到这儿那就应该是这个了
        if (m_connectedNode)
        if (conn.m_key == m_connectedNodeInfo.m_key)
        {
            nNodeId = m_connectedNode;
            nVip    = 0;
            return SUCCESS;
        }
        return OUT_OF_RANGE;
    }
    //如果有老的连接，会在这儿返回给CommMgr
    CCommConnection *OnConnected(CCommConnection *pCommConnection,TUInt32 uTimeNow)
    {
        if (pCommConnection->GetNodeId() == m_localNodeId)
        {
            return m_pVirtualIps[pCommConnection->GetVirtualIp()].OnConnected(pCommConnection,uTimeNow);
        } 
        //if (pCommConnection->GetNodeId() == m_connectedNode) 必须是，不然就奇怪了
        return m_pVirtualIps[m_nrOfVirtualIp].OnConnected(pCommConnection,uTimeNow);
    }
    
    void OnDisconnected(CCommConnection *pCommConnection,TUInt32 uTimeNow)
    {
        if (pCommConnection->GetNodeId() == m_localNodeId)
        {
            return m_pVirtualIps[pCommConnection->GetVirtualIp()].OnDisconnected(uTimeNow);
        } 
        //if (pCommConnection->GetNodeId() == m_connectedNode) 必须是，不然就奇怪了
        return m_pVirtualIps[m_nrOfVirtualIp].OnDisconnected(uTimeNow);
    }

    //返回
    CIpMapItem *GetConnection(TUInt32 nIdx)
    {
        return m_pVirtualIps + nIdx;
    }

    TInt32 Init(const TChar *pConfigName,IfConnection *pSelf);
    TBool IsLocal(CDoid *pDoId)
    {
        if((pDoId->m_nodeId == m_localNodeId) && (pDoId->m_virtualIp == m_localVirtualIp))
        {
            return TRUE;
        }
        return FALSE;
    }

    IfConnection *RouteTo(CDoid *pDoId)
    {
        if (pDoId->m_nodeId < m_nrOfNodes)
        {
            if (pDoId->m_nodeId == m_localNodeId)
            {
                if (pDoId->m_virtualIp < m_nrOfVirtualIp)
                {
                    return m_pVirtualIps[pDoId->m_virtualIp].m_pIfConnection;
                }
            }
            else
            {
                if (pDoId->m_nodeId == m_connectedNode)
                {
                    return m_pVirtualIps[m_redirectIdx].m_pIfConnection;
                }
                //else
                return m_pVirtualIps[m_pRoutes[pDoId->m_nodeId]].m_pIfConnection;
            }
            //return m_pRoutes[pDoId->m_nodeId];
        }
        //直接丢弃.
        return NULL;
    }
};

}
#endif

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



class CIpMapItem
{
public:
    CIpMapItem()
    {
        m_nNodeId = 0;
        m_nVirtualIp = 0;
        m_uLastUsedTime = 0;
        m_pConnection = 0;
        m_pIfConnection = 0;
        m_pListeningItem = 0;
    }
    //�����һ��ʼ�ͳ�ʼ���õ�

    CConPair            m_tKey;
    TUInt16             m_nNodeId;
    TUInt16             m_nVirtualIp;
    TUInt32             m_uLastUsedTime;
    CCommConnection     *m_pConnection;
    IfConnection        *m_pIfConnection;
    void                *m_pListeningItem;
    void OnConnecting(CCommConnection *pCon,TUInt32 uTimeNow)
    {
        m_pConnection = pCon;
        m_uLastUsedTime = uTimeNow;
    }
    //�������ӵĵ��������������һ��CCommConnection
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
    //��д
    TUInt16              m_nrOfNodes;
    TUInt16              m_nrOfVirtualIp;
    TUInt16              m_localNodeId;
    TUInt16              m_localVirtualIp;
    CIpMapItem           *m_pVirtualIps;
    TUInt32              m_nNrOfMapItem;
    //ת��·�ɱ�,����ͬ��node.
    TUInt32              *m_pRoutes;

    TUInt16              *m_pListening;
    TUInt32              m_nNrOfLisenting;
    
    //CCommConnection      *m_pLocalConnections;
public:
    CIpMap();

    TBool IsPostive(TInt32 idx)
    {
        CIpMapItem *pItem = m_pVirtualIps + idx;
        if (pItem->m_nNodeId < m_localNodeId)
        {
            return TRUE;
        }
        if (pItem->m_nNodeId == m_localNodeId)
        {
            if (pItem->m_nVirtualIp < m_localVirtualIp)
            {
                return TRUE;
            }
        }
        return FALSE;
    }
    //ֻ�б������ӵ��������
    CIpMapItem *GetIpMapInfo(CConPair *pConn)
    {
        for (TUInt32 i = 0;i<m_nNrOfMapItem;++i)
        {
            if (*pConn == m_pVirtualIps[i].m_tKey)
            {
                return m_pVirtualIps + i;
            }
        }
        //�ߵ�����Ǿ�Ӧ���������
        return NULL;
    }
    //������ϵ����ӣ�����������ظ�CommMgr
    CCommConnection *OnConnected(CCommConnection *pCommConnection,TUInt32 uTimeNow)
    {
        if (pCommConnection->GetNodeId() == m_localNodeId)
        {
            return m_pVirtualIps[pCommConnection->GetVirtualIp()].OnConnected(pCommConnection,uTimeNow);
        } 
        //if (pCommConnection->GetNodeId() == m_connectedNode) �����ǣ���Ȼ�������
        //if (pCommConnection->GetNodeId() < m_nrOfNodes) //�϶���
        {
            return m_pVirtualIps[m_pRoutes[pCommConnection->GetNodeId()]].OnConnected(pCommConnection,uTimeNow);
        }
    }
    
    void OnDisconnected(CCommConnection *pCommConnection,TUInt32 uTimeNow)
    {
        if (pCommConnection->GetNodeId() == m_localNodeId)
        {
            m_pVirtualIps[pCommConnection->GetVirtualIp()].OnDisconnected(uTimeNow);
            return;
        } 
        //if (pCommConnection->GetNodeId() == m_connectedNode) �����ǣ���Ȼ�������
        m_pVirtualIps[m_pRoutes[pCommConnection->GetNodeId()]].OnDisconnected(uTimeNow);
    }
    
    int CIpMap::ReadIpMapItem(void *pFile,char *pMain,CIpMapItem *pItem);

    //����
    CIpMapItem *GetConnection(TUInt32 nIdx)
    {
        return m_pVirtualIps + nIdx;
    }

    TInt32 Init(const TChar *pConfigName,IfConnection *pSelf);
    TBool IsLocal(CIpMapItem *pItem)
    {
        if ((pItem->m_nNodeId == m_localNodeId)&&(pItem->m_nVirtualIp == m_localVirtualIp))
        {
            return TRUE;
        }
        return FALSE;
    }
    TBool IsLocal(CDoid *pDoId)
    {
        if((pDoId->m_nodeId == m_localNodeId) && (pDoId->m_virtualIp == m_localVirtualIp))
        {
            return TRUE;
        }
        return FALSE;
    }

    CIpMapItem *RouteTo(CDoid *pDoId)
    {
        if (pDoId->m_nodeId < m_nrOfNodes)
        {
            if (pDoId->m_nodeId == m_localNodeId)
            {
                if (pDoId->m_virtualIp < m_nrOfVirtualIp)
                {
                    return &m_pVirtualIps[pDoId->m_virtualIp];
                }
            }
            else
            {
                //else
                return &m_pVirtualIps[m_pRoutes[pDoId->m_nodeId]];
            }
            //return m_pRoutes[pDoId->m_nodeId];
        }
        //ֱ�Ӷ���.
        return NULL;
    }
private:
    TBool IsListeningExisted(CIpMapItem *pItem,TInt32 nMax)
    {
        for (TInt32 i=0;i<nMax;++i)
        {
            if (m_pVirtualIps[i].m_tKey.GetMyIp() == pItem->m_tKey.GetMyIp())
            {
                if (m_pVirtualIps[i].m_tKey.GetMyPort() == pItem->m_tKey.GetMyPort())
                {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
};

}
#endif

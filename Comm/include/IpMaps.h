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
class CIpMap
{
private:
    TUInt16              m_nrOfNodes;
    TUInt16              m_nrOfVirtualIp;
    TUInt16              m_myNodeId;
    TUInt16              m_localVirtualIp;
    CCommConnection      *m_pLocalConnections;
public:
    CIpMap();
    void Init(CCommConnection *pConnections,TUInt16 nrOfNode,TUInt16 nrOfVirtual,TUInt16 myNodeId,TUInt16 localVirtualIp);
    TBool IsLocal(CDoid *pDoId)
    {
        if((pDoId->m_nodeId == m_myNodeId) && (pDoId->m_virtualIp == m_localVirtualIp))
        {
            return TRUE;
        }
        return FALSE;
    }
    CCommConnection *GetConnection(CDoid *pDoid)
    {
        //ֻ���nodeId,��srvId
        if((pDoid->m_nodeId == m_myNodeId))
        {
            if ((pDoid->m_virtualIp != m_localVirtualIp))
            {
                if (pDoid->m_virtualIp < m_nrOfVirtualIp)
                {
                    return (m_pLocalConnections + pDoid->m_virtualIp);
                }
            }
        }
        else
        {
            return m_pLocalConnections;
        }
        return NULL;
    }
};
}
#endif

#include "IpMaps.h"

namespace Zephyr
{

CIpMap::CIpMap()
{
    m_nrOfNodes = 0;
    m_nrOfVirtualIp = 0;
    m_myNodeId = 0;
    m_localVirtualIp = 0;
    m_pLocalConnections = NULL;
}

void CIpMap::Init(CCommConnection *pConnections,TUInt16 nrOfNode,TUInt16 nrOfVirtual,TUInt16 myNodeId,TUInt16 localVirtualIp)
{
    m_nrOfNodes = nrOfNode;
    m_nrOfVirtualIp = nrOfVirtual;
    m_myNodeId = myNodeId;
    m_localVirtualIp = localVirtualIp;
    m_pLocalConnections = pConnections;
}

}

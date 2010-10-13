#include "Doid.h"
#include "SysMacros.h"
#include <stdio.h>

namespace Zephyr
{

void CDoid::ToStr(TChar *pBuff)
{
    sprintf(pBuff,"Doid://%u/%u/%u/%u",m_nodeId,m_virtualIp,m_srvId,m_objId);
}

}


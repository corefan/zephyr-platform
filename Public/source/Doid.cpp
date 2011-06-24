#include "Doid.h"
#include "SysMacros.h"
#include <stdio.h>

namespace Zephyr
{

void CDoid::ToStr(TChar *pBuff)
{
    sprintf(pBuff,"Doid://%u/%u/%u/%u",(unsigned int)m_nodeId,(unsigned int)m_virtualIp,(unsigned int)m_srvId,(unsigned int)m_objId);
}

}


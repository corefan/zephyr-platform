#include "../include/DistributedObject.h"

namespace Zephyr
{

CObject::CObject()
{
    m_pSkeleton = NULL;
    m_pCurrentMsg = NULL;

    m_nMsgHandled = 0;
    m_nMsgSent = 0;
    m_nMsgDroped = 0;
    m_nStartTime = 0;
}

TInt32      CObject::OnInit()
{
    m_nStartTime = 0;
    return SUCCESS;
}

TInt32 CObject::OnFinal()
{
    return SUCCESS;
}


}
#ifndef __ZEPHYR_PUBLIC_COMM_CENTER_H__
#define __ZEPHYR_PUBLIC_COMM_CENTER_H__
#include "IfCommunicator.h"
#include "IfTaskMgr.h"
#include "IfLoggerMgr.h"

namespace Zephyr
{
IfCommunicatorMgr *CreateCommMgr(IfTaskMgr *pTaskMgr,IfLoggerManager *pLoggerMgr);
}


#endif

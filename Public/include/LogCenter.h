#ifndef __ZEPHYR_PUBLIC_LOG_CENTER_H__
#define __ZEPHYR_PUBLIC_LOG_CENTER_H__

#include "IfLoggerMgr.h"
#include "IfLogger.h"
#include "IfTaskMgr.h"

namespace Zephyr
{

IfLoggerManager *CreateLogSys(IfTaskMgr *pTaskMgr);


}

#endif

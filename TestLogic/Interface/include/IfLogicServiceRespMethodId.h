#ifndef __IFLOGICSERVICERESP_METHOD_ID_H__
#define __IFLOGICSERVICERESP_METHOD_ID_H__
#include "../../include/ServiceIdDef.h"

#define IFLOGICSERVICERESP_INTERFACE_ID ((0x00000001) * 200) 
#define IfLogicServiceRespServiceId (TEST_LOGIC_SERVICE_ID*1000)
#define IfLogicServiceRespServiceIdBegin RESPGETSESSION_TUINT64_CDOID_ID
#define RESPGETSESSION_TUINT64_CDOID_ID ((TEST_LOGIC_SERVICE_ID*1000)+IFLOGICSERVICERESP_INTERFACE_ID+(0))
#define RESPRELEASESESSION_TUINT64_ID ((TEST_LOGIC_SERVICE_ID*1000)+IFLOGICSERVICERESP_INTERFACE_ID+(1))
#define IfLogicServiceRespServiceIdEnd ((TEST_LOGIC_SERVICE_ID*1000)+IFLOGICSERVICERESP_INTERFACE_ID+(0x00000002))
#endif


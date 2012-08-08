#ifndef __IFLOGIC_METHOD_ID_H__
#define __IFLOGIC_METHOD_ID_H__
#include "../../include/ServiceIdDef.h"

#define IFLOGIC_INTERFACE_ID ((0x00000002) * 200) 
#define IfLogicServiceId (TEST_LOGIC_SERVICE_ID*1000)
#define IfLogicServiceIdBegin TEST_TUINT64_TINT32_ID
#define TEST_TUINT64_TINT32_ID ((TEST_LOGIC_SERVICE_ID*1000)+IFLOGIC_INTERFACE_ID+(0))
#define IfLogicServiceIdEnd ((TEST_LOGIC_SERVICE_ID*1000)+IFLOGIC_INTERFACE_ID+(0x00000001))
#endif


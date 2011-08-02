#ifndef __ZEPHYR_GATEWAY_LOGGER_H__
#define __ZEPHYR_GATEWAY_LOGGER_H__

#include "Public/Interface/Platform/include/IfLogger.h"

#define LOG_DEBUG(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_debug,Format,__VA_ARGS__)

#define LOG_CRITICAL(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_critical,Format,__VA_ARGS__)

#define LOG_RUN(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_run,Format,__VA_ARGS__)

#define LOG_TEST(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_test,Format,__VA_ARGS__)


enum EnGWError
{
    en_gw_error_begin    = 0x00010000, //每个模块预留64k个，绝对够了
    en_error_service_id,
    en_allocate_route_mem_failed,
};

#endif

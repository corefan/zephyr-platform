#ifndef __ZEPHYR_GATEWAY_LOGGER_H__
#define __ZEPHYR_GATEWAY_LOGGER_H__

#include "Public/Interface/Platform/include/IfLogger.h"
//这里的Format不能是char*指针，必须是字符串"",就能自动带入File和Line
#define LOG_DEBUG(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_debug,"[File:%s,Line:%u]"##Format,__FILE__,__LINE__,__VA_ARGS__)

//这里的Format不能是char*指针，必须是字符串"",就能自动带入File和Line
#define LOG_CRITICAL(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_critical,"[File:%s,Line:%u]"##Format,__FILE__,__LINE__,__VA_ARGS__)

//每一个运行日志都必须有自己的日志号，这样，可以根据日志号，直接找到错误点.
#define LOG_RUN(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_run,Format,__VA_ARGS__)

//这里的Format不能是char*指针，必须是字符串"",就能自动带入File和Line
#define LOG_TEST(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_test,"[File:%s,Line:%u]"##Format,__FILE__,__LINE__,__VA_ARGS__)


enum EnGWLogId
{
    //前65536个保留给系统，这些错误是需要写critical日志的，属于系统错误，而下面的是运行错误，说明输入参数有误.
    //所以查日志只要查日志号小于65536的，其它都是属于使用错误。
    en_gw_error_begin    = 0x00010000, //每个模块预留64k个，绝对够了
    en_error_service_id,
    en_allocate_route_mem_failed,
    en_route_info_not_found,
    en_start_listing,
    en_start_listening_failed,
    en_restart_listening,
    en_stop_listening,
    en_listening_not_started,
    en_no_much_service_id_found,
};

#endif

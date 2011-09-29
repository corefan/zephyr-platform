#ifndef __ZEPHYR_AUTHENTICATE_SERVICE_LOGGER_H__
#define __ZEPHYR_AUTHENTICATE_SERVICE_LOGGER_H__


#include "Public/Interface/Platform/include/IfLogger.h"

enum EnASLogId
{
    //前65536个保留给系统，这些错误是需要写critical日志的，属于系统错误，而下面的是运行错误，说明输入参数有误.
    //所以查日志只要查日志号小于65536的，其它都是属于使用错误。
    en_as_error_begin    = 0x00030000, //每个模块预留64k个，绝对够了

};



#endif

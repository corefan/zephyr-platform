#ifndef __ZEPHYR_AUTHENTICATE_LOGGER_H__
#define __ZEPHYR_AUTHENTICATE_LOGGER_H__


#include "Public/Interface/Platform/include/IfLogger.h"

namespace erp_platform
{
    enum EnDbLoggerError
    {
        en_client_disconneted = 0x00020000, //前0x00010000留给了gateway.
        en_reading_db,               //正在读数据库，请稍后
        en_system_is_too_busy,       //系统太忙，请稍后再试
        en_incorrect_data_length,    //接收到的数据长度不对
        en_statistic_db_trans_num,
        en_data_trans_failed, //数据库操作失败
    };
}

#endif

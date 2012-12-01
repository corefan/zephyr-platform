#ifndef __ZEPHYR_AUTHENTICATE_LOGGER_H__
#define __ZEPHYR_AUTHENTICATE_LOGGER_H__


#include "Public/Interface/Platform/include/IfLogger.h"

namespace erp_platform
{
    enum EnDbLoggerError
    {
        en_client_disconneted = 0x00020000, //ǰ0x00010000������gateway.
        en_reading_db,               //���ڶ����ݿ⣬���Ժ�
        en_system_is_too_busy,       //ϵͳ̫æ�����Ժ�����
        en_incorrect_data_length,    //���յ������ݳ��Ȳ���
        en_statistic_db_trans_num,
        en_data_trans_failed, //���ݿ����ʧ��
    };
}

#endif

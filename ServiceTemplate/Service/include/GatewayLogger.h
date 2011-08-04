#ifndef __ZEPHYR_GATEWAY_LOGGER_H__
#define __ZEPHYR_GATEWAY_LOGGER_H__

#include "Public/Interface/Platform/include/IfLogger.h"
//�����Format������char*ָ�룬�������ַ���"",�����Զ�����File��Line
#define LOG_DEBUG(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_debug,"[File:%s,Line:%u]"##Format,__FILE__,__LINE__,__VA_ARGS__)

//�����Format������char*ָ�룬�������ַ���"",�����Զ�����File��Line
#define LOG_CRITICAL(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_critical,"[File:%s,Line:%u]"##Format,__FILE__,__LINE__,__VA_ARGS__)

//ÿһ��������־���������Լ�����־�ţ����������Ը�����־�ţ�ֱ���ҵ������.
#define LOG_RUN(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_run,Format,__VA_ARGS__)

//�����Format������char*ָ�룬�������ַ���"",�����Զ�����File��Line
#define LOG_TEST(LogId,Format,...)	GetLogger()->WriteLog(LogId,log_test,"[File:%s,Line:%u]"##Format,__FILE__,__LINE__,__VA_ARGS__)


enum EnGWLogId
{
    //ǰ65536��������ϵͳ����Щ��������Ҫдcritical��־�ģ�����ϵͳ���󣬶�����������д���˵�������������.
    //���Բ���־ֻҪ����־��С��65536�ģ�������������ʹ�ô���
    en_gw_error_begin    = 0x00010000, //ÿ��ģ��Ԥ��64k�������Թ���
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

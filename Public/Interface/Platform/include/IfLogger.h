#ifndef __ZEPHYR_SYSTEM_LOGGER_IF_LOGGER_H__
#define __ZEPHYR_SYSTEM_LOGGER_IF_LOGGER_H__
#include "stdarg.h"
#include "../../../include/TypeDef.h"

namespace Zephyr
{
#define DEFAULT_CACHED_LOG_LENGTH (1024 * 1024)

enum EnLogLvl
{
    log_critical     = 1,               //����������ʱ���������󣬱����ڴ治��������Ͽ�
    log_run          = (1<<1),          //������־��������Ʒ���������񴴽���
    log_test         = (1<<2),          //����ģʽ
    log_debug        = (1<<3),          //debug��־����release���в�����.
    log_release_mode = (log_critical|log_run),
    log_test_mode    = (log_critical|log_run|log_test),
    log_debug_mode   = (log_critical|log_run|log_debug),
};

class IfLogger
{
public:
    virtual void WriteLog(const TUInt32 logId,const TUInt32 lvl,const TChar *_pFormat,...) = 0;
    virtual void WriteLog(const TUInt32 lvl,const TChar *_pFormat,...) = 0;
    virtual void WriteLog(const TUInt32 logId,const TUInt32 lvl,const TChar* __pFormat,va_list vl) = 0;
    virtual void WriteLog(const TUInt32 lvl,const TChar *__pFormat,va_list vl) = 0;
    //ֱ��д���ݣ�������ʽд
    virtual void WriteRawLog(const TUInt32 lvl,const TChar *__pFormat,...) = 0;

    //ֱ��д����������Ҫ�����.
    virtual void WriteBinLog(const TChar *pBin,TUInt32 uLength) = 0;
};


}


#endif

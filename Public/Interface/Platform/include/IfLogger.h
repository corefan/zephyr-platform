#ifndef __ZEPHYR_SYSTEM_LOGGER_IF_LOGGER_H__
#define __ZEPHYR_SYSTEM_LOGGER_IF_LOGGER_H__
#include "stdarg.h"
#include "TypeDef.h"

namespace Zephyr
{
#define DEFAULT_CACHED_LOG_LENGTH (1024 * 1024)

enum EnLogLvl
{
    log_warning     = 1,
    log_run         = (1<<1),
    log_critical    = (1<<2),
    log_debug       = (1<<3),
};

class IfLogger
{
public:
    virtual void WriteLog(const TUInt32 logId,const TUInt32 lvl,const TChar *_pFormat,...) = 0;
    virtual void WriteLog(const TUInt32 lvl,const TChar *_pFormat,...) = 0;
    virtual void WriteLog(const TUInt32 logId,const TUInt32 lvl,const TChar* __pFormat,va_list vl) = 0;
    virtual void WriteLog(const TUInt32 lvl,const TChar *__pFormat,va_list vl) = 0;
    //直接写数据，不按格式写
    virtual void WriteRawLog(const TUInt32 lvl,const TChar *__pFormat,...) = 0;

    //直接写比特流，不要随便用.
    virtual void WriteBinLog(const TChar *pBin,TUInt32 uLength) = 0;
};


}


#endif

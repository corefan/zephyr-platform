#include "IfLogger.h"
namespace Zephyr
{


IfLogger *g_pCommLogger;

#define COMM_DEBUG_LOG(LOG_IDX,LOG_STR,...) g_pCommLogger->WriteLog((LOG_IDX),log_debug,LOG_STR,__VA_ARGS__);

#define COMM_RUN_LOG(LOG_IDX,LOG_STR,...) g_pCommLogger->WriteLog(LOG_IDX,log_run,LOG_STR,__VA_ARGS__);

#define COMM_EXCPETION_LOG(LOG_IDX,LOG_STR,...) g_pCommLogger->WriteLog(LOG_IDX,log_critical,LOG_STR,__VA_ARGS__);

#define COMM_CONNECTION_REINITED 1

}
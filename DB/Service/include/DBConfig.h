#ifndef __ZEPHYR_DB_CONFIG_H__
#define __ZEPHYR_DB_CONFIG_H__

#include "Public/include/TypeDef.h"
#include "Public/Config/include/SettingFile.h"

namespace erp_platform
{

#define MAX_FILE_NAME_LENGTH (32)
#define MAX_CONNECT_STR_LEN (256)
//connection_str=server=172.2.2.2;Port3306;UID=root;PWD=pwd;DB=db01;FLAG=CLIENT_STATEMENTS|CLIENT_MULTi_RESULTS


class CDBConfig
{
public:
    TChar   m_szLoggerName[MAX_FILE_NAME_LENGTH];
    TChar   m_szConnectStr[MAX_CONNECT_STR_LEN];
    TInt32  m_nThreadCount;//=DEFAULT_TRANS_THREAD,int QueueSize,unsigned int Flag=0
    TInt32  m_nQueueSize; //=DEFAULT_TRANS_QUEUE
    TInt32  m_nFlag; //0;
public:
    CDBConfig()
    {
        strncpy_s(m_szLoggerName,"DB_Logger",MAX_FILE_NAME_LENGTH);
        strncpy_s(m_szConnectStr,"",MAX_CONNECT_STR_LEN);
    }
    TInt32    ReadConfig(TChar *pszConfigName)
    {
        CSettingFile tFile;
        if (tFile.LoadFromFile(pszConfigName))
        {
            const char *psz = tFile.GetString("MAIN","logger_name","as_logger");
            if (psz)
            {
                strncpy_s(m_szLoggerName,psz,MAX_FILE_NAME_LENGTH);
            }
            psz = tFile.GetString("MAIN","connect_str","");
            if (psz)
            {
                strncpy_s(m_szConnectStr,psz,MAX_CONNECT_STR_LEN);
            }
            m_nThreadCount = tFile.GetInteger("MAIN","thread_count",4);
            m_nQueueSize = tFile.GetInteger("MAIN","queue_size",512);
            m_nFlag      = tFile.GetInteger("MAIN"," ",0);
            return 0;
        }
        return -1;
    }
};


}

#endif

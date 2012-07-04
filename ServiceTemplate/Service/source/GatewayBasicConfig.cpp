#include "../include/GatewayBasicConfig.h"
#include "Public/Config/include/SettingFile.h"
#include "Public/include/SysMacros.h"
#include "Public/Interface/Platform/include/IfLogger.h"
#include "Public/include/TypeDef.h"
#include "winsock2.h"
namespace Zephyr
{

CGatewayBasicConfig::CGatewayBasicConfig()
{
    m_uMaxConnections = 0;
    m_uMaxIncomingConnection4Listner = 0;
    m_uOutPutCacheInKBs = 0;
    m_uInputCacheInKBs = 0;
    m_szLoggerName[0] = 0;
#ifdef _DEBUG
    m_uWriteLoggerMask = log_debug_mode;
    m_uPrint2ScreenLoggerMask = log_debug_mode; 
#else
    m_uWriteLoggerMask = log_release_mode;
    m_uPrint2ScreenLoggerMask = log_release_mode;
#endif
}
    //�����䱻���ص�ID����ȡ����.
TInt32  CGatewayBasicConfig::ReadFile(TUInt16 uVIp,TUInt16 uServiceId)
{
    char szFileName[64];
    sprintf_s(szFileName,sizeof(szFileName),"%d/GatewayConfig%d.ini",uVIp,uServiceId);
    CSettingFile tFile;
    if (tFile.LoadFromFile(szFileName))
    {
        char szMain[] = "MAIN";
        m_uMaxConnections = tFile.GetInteger(szMain,"max_connection",2200);
        m_uMaxIncomingConnection4Listner = tFile.GetInteger(szMain,"max_incoming_connecting_4_listener",32);
        m_uOutPutCacheInKBs = tFile.GetInteger(szMain,"output_cache_in_kbs",64);
        m_uInputCacheInKBs  = tFile.GetInteger(szMain,"input_cache_in_kbs",16);
        const char *psz = tFile.GetString(szMain,"logger_name","gateway_logger");
        strncpy_s(m_szLoggerName,psz,sizeof(m_szLoggerName));
        TInt32 nTmp = tFile.GetInteger("AS","node_id",-1);
        if (nTmp < 0)
        {
            return FAIL;
        }
        m_tASDoid.m_nodeId = nTmp;
        nTmp = tFile.GetInteger("AS","virtual_ip",-1);
        if (nTmp < 0)
        {
            return FAIL;
        }
        m_tASDoid.m_virtualIp = nTmp;
        nTmp = tFile.GetInteger("AS","svr_id",-1);
        if (nTmp < 0)
        {
            return FAIL;
        }
        m_tASDoid.m_srvId = nTmp;
        m_tASDoid.m_objId = 0;
        m_uListeningPort = tFile.GetInteger(szMain,"Port",(12222+uVIp*10+uServiceId));
        const TChar *pIp = tFile.GetString(szMain,"IP","0");
        m_uListeningIp = inet_addr(pIp);
        return SUCCESS;
    }
    return FAIL;
}

}
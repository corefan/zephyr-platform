#include "LoggerManager.h"


namespace Zephyr
{

CLoggerManager* CLoggerManager::sm_pInstance = NULL;

CLoggerManager::CLoggerManager()
{
    Init();
}

TInt32 CLoggerManager::Init()
{
    for (TInt32 i=0;i<MAX_LOGGER_NUM;i++)
    {
        m_pLogger[i] = NULL;
    }
    return SUCCESS;
}

IfLogger *CLoggerManager::AddLogger(const TChar *pName,TUInt32 loggerIdx,TUInt32 logLvlWriteFileMask/* = 0xFFFFFFFF*/,TUInt32 logLvlPrintScreenMask /*= 0*/,TUInt32 cacheLen/* = DEFAULT_CACHED_LOG_LENGTH*/)
{
    if (m_pLogger[loggerIdx])
    {
        //already existed!
        return m_pLogger[loggerIdx];
    }
    CLogger *pLogger = new CLogger();
    TInt32 ret = pLogger->Init(pName,logLvlWriteFileMask,logLvlPrintScreenMask,cacheLen);
    if (ret < SUCCESS)
    {
        delete pLogger;
        //init failed
        return NULL;
    }
    m_pLogger[loggerIdx] = pLogger;
    return pLogger;
}

void CLoggerManager::ReleaseLogger(IfLogger *pLogger)
{
    for(TInt32 i=0;i<MAX_LOGGER_NUM;i++)
    {
        if (m_pLogger[i] == pLogger)
        {
            delete m_pLogger[i];
            m_pLogger[i] = NULL;
            //return SUCCESS;
        }
    }
    //return NOT_INITIALIZED;
}


void CLoggerManager::OnException(FILE *pDump)
{
    if (pDump)
    {
        char dumpBegin[] = "Begin to dump LoggerManger!";
        dumpBegin[sizeof(dumpBegin)-1] = '\n';
        fwrite(dumpBegin, sizeof(dumpBegin),1,pDump);
        fflush(pDump);
    }
    for (TInt32 i = 0;i<MAX_LOGGER_NUM;i++)
    {
        if (m_pLogger[i])
        {
            m_pLogger[i]->OnException(pDump);
        }
    }
    if (pDump)
    {
        char dumpSucces[] = "LoggerManger has dumped successful !";
        dumpSucces[sizeof(dumpSucces)-1] = '\n';
        fwrite(dumpSucces, sizeof(dumpSucces),1,pDump);
        fflush(pDump);
    }
    printf("LoggerManger has dumped!");
}

TInt32 CLoggerManager::Run(const TInt32 threadId,const TInt32 runCnt)
{
    TInt32 usedCnt = 0;
    
    time_t rawTime;
    struct tm * pTimeInfo;

    time ( &rawTime );

    pTimeInfo = localtime ( &rawTime );
    
    for (TInt32 i = 0;i<MAX_LOGGER_NUM;i++)
    {
        if (m_pLogger[i])
        {
            usedCnt += m_pLogger[i]->Run(runCnt,pTimeInfo);
        }
    }
    #ifdef _DEBUG
    if (usedCnt)
    {
        Sleep(15);
        usedCnt ++;
    }
    #endif
    return usedCnt;
}

void CLoggerManager::WriteLog(const TUInt32 loggerIdx,const TUInt32 logId,const TUInt32 logLvl,const TChar *__pFormat,...) 
{
    if (loggerIdx < MAX_LOGGER_NUM)
    {
        
    }
}

}

#ifndef __DB_LIB_DB_TRASATION_MANAGER_H__
#define __DB_LIB_DB_TRASATION_MANAGER_H__


#include "DBLib.h"

#include "Public/Interface/Platform/include/IfLogger.h"
#include "System/include/Lock.h"

using namespace Zephyr;

namespace DBLib
{



class CDBTransationManager : public IfTrascationWorkThreadMgr
{
public:
	enum FLAG
	{
		DBTM_FLAG_LOG_PERFORMANCE=1,
	};
    class CDbLoggerWithLock : public IfLogger
    {
    public:
        CDbLoggerWithLock()
        {
            m_pLogger = NULL;
        }
        void SetLogger(IfLogger *pLogger)
        {
            m_tLocks.Lock();
            m_pLogger = pLogger;
            m_tLocks.Unlock();
        }
        IfLogger *m_pLogger;
        CLock                                   m_tLocks;
        virtual void WriteLog(const TUInt32 logId,const TUInt32 lvl,const TChar *_pFormat,...);
        virtual void WriteLog(const TUInt32 lvl,const TChar *_pFormat,...);
        virtual void WriteLog(const TUInt32 logId,const TUInt32 lvl,const TChar* __pFormat,va_list vl);
        virtual void WriteLog(const TUInt32 lvl,const TChar *__pFormat,va_list vl);
        //直接写数据，不按格式写
        virtual void WriteRawLog(const TUInt32 lvl,const TChar *__pFormat,...);
        virtual void WriteRawLog(const TUInt32 lvl,const TChar *__pFormat,va_list vl);
        //直接写比特流，不要随便用.
        virtual void WriteBinLog(const TChar *pBin,TUInt32 uLength);
    };
protected:
	std::vector<CDBTransationWorkThread *>	m_WorkThreads;
	IDatabase *								m_pDatabase;
	UINT									m_Flag;

	CEasyTimer								m_PerformanceCountTimer;
	volatile UINT							m_ExecTimes;
	volatile UINT							m_RealExecTime;

	float									m_AvgExecTime;
	float									m_ExecTimesPerSec;

	CDbLoggerWithLock                       m_tLogger;
	
public:
    void SetLogger(IfLogger *pLogger)
    {
        m_tLogger.SetLogger(pLogger);
    }
    
    IfLogger *GetLogger()
    {
        return &m_tLogger;
    }

	CDBTransationManager(void);
	~CDBTransationManager(void);

    IDatabase *GetDB()
    {
        return m_pDatabase;
    }
	void Destory();

	bool Init(IDatabase * pDatabase,LPCTSTR szConnectStr,IfLogger *pLogger,int ThreadCount=DEFAULT_TRANS_THREAD,int QueueSize=DEFAULT_TRANS_QUEUE,UINT Flag=0);

	void ModifyFlag(UINT Add,UINT Remove)
	{
		m_Flag|=Add;
		m_Flag&=~Remove;
	}

	CDBTransationWorkThread * AllocWorkThread();
	
	bool AddTransaction(CDBTransaction * pDBTansaction);

    virtual  IfTrasactionWorkThread *GetThread();
    virtual  IfTrasactionWorkThread *GetThread(int nIdx);
    
	virtual int Update(int ProcessLimit=DEFAULT_PROCESS_LIMIT);

	bool IsIdle();

	void AddExecTime(UINT ExecTime)
	{
		m_ExecTimes++;
		m_RealExecTime+=ExecTime;
	}

	float GetAvgExecTime()
	{
		return m_AvgExecTime;
	}
	float GetExecTimesPerSec()
	{
		return m_ExecTimesPerSec;
	}

};

}

#endif

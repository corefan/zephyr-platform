/*-------------------------------------------------------------
 Copyright (C)| 
 File: TaskMgr.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  ���������
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_TASK_MGR_H__
#define __ZEPHYR_TASK_MGR_H__
#include "TypeDef.h"
#include "IfTask.h"
#include "IfTaskMgr.h"
#include "SysMacros.h"
#include "Lock.h"

namespace Zephyr
{

class CTaskInfo
{
private:
    volatile TInt32 m_operatingThread;
    CLock   m_lock;
    IfTask  *m_pTask;
    EnTaskPriority m_priority;
public:

    CTaskInfo():m_lock()
    {
        m_pTask = NULL;
        m_operatingThread = 0;
        m_priority = insignificant_task;
    }
    ~CTaskInfo()
    {
        
    }
    IfTask *GetTask()
    {
        return m_pTask;
    }
    TBool IsUsed()
    {
         if (m_pTask)
         {
            return TRUE;
         }
         return FALSE;
    }
    TInt32 LoadTask(IfTask *pTask ,EnTaskPriority priority )
    {
        m_lock.Lock();
        if (m_pTask)
        {
            m_lock.Unlock();
            return TASK_ALREADY_INITED;
        }
        m_pTask = pTask;
        m_priority = priority;
        m_lock.Unlock();
        return SUCCESS;
    }
    void UnloadTask()
    {
        m_lock.Lock();
        if (m_pTask)
        {
            m_pTask = NULL;
        }
        m_lock.Unlock();
        return;
    }
    //++m_operatingThread;
    TInt32 ApplyTask(TInt32 threadId);
    TInt32 Run(TInt32 threadId);
    //--m_operatingThread
    TInt32 QuitTask();


};

class CTaskWorkers
{
protected:
    volatile TUInt32     m_workerThreadHandle;
    CTaskInfo   *m_pTaskInfo;
    TInt32      m_threadIdx;
    TInt32      m_threadSleepGap;
public:
    ~CTaskWorkers();
    static void Run(void *pArg);
    void Loop();
    TInt32  Start(CTaskInfo *pInfo,TInt32 threadId,TInt32 sleepGap);
    //this will block the thread as it will wait the Loop to stop.
    void    TryStop()
    {
        m_workerThreadHandle = 0;
    }
    TInt32  Stop();
};
//U can have 32 tasks at max!
class CTaskMgr : public IfTaskMgr
{
protected:
    //max 32 task!
    CTaskInfo m_taskList[MAX_TASK_NR_IN_MGR];
    TUInt16 m_nrOfTask;
    TUInt16 m_nrOfWorker;
    CLock      m_listLock;
    CTaskWorkers *m_pWorkers;
public:
    //call in the following order.
    CTaskMgr();
    
    TInt32 AddTask(IfTask *pTask,EnTaskPriority priority);
    TInt32 StartWorking(TInt32 nrOfWorkersNeeded);
    TInt32 StopWorking();
    TInt32 ReleaseTask(IfTask *pTask);

    ~CTaskMgr();
};
        
    
};

#endif

/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfTaskMgr.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  任务管理器接口
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_IF_TASK_MGR_H__
#define __ZEPHYR_IF_TASK_MGR_H__
#include "Typedef.h"
#include "IfTask.h"




namespace Zephyr
{
    
class IfTaskMgr
{
public:
	virtual TInt32 StartWorking(TInt32 nrOfWorkersNeeded) = 0;
    
    virtual TInt32 AddTask(IfTask *pTask,EnTaskPriority priority) = 0;

    virtual TInt32 ReleaseTask(IfTask *pTask) = 0;

	virtual TInt32 StopWorking() = 0;
};

}


#endif

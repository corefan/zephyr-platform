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
    //几个工作线程
	virtual TInt32 StartWorking(TInt32 nrOfWorkersNeeded) = 0;
    //增加一个工作，最好在startworking前调用
    virtual TInt32 AddTask(IfTask *pTask,EnTaskPriority priority = normal_task) = 0;
    //减去一个工作，最好在startworking前调用
    virtual TInt32 ReleaseTask(IfTask *pTask) = 0;
    //停止工作
	virtual TInt32 StopWorking() = 0;
};

}


#endif

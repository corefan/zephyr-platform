/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfTask.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  任务接口
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_IF_TASK_H__
#define __ZEPHYR_IF_TASK_H__


 
#include "TypeDef.h"
#include "SysMacros.h"
namespace Zephyr
{
    
enum EnTaskPriority
{
    insignificant_task      = 0x00,          //比其它task获得更少的runCnt
    normal_task             = 0x01,         //轮询调用
    very_urgent_task        = 0x02,         //每当有CPU 就会调用这个task,即会一直调用知道该线程无事可做
};

class IfTask
{
private:
    void *m_pData;
public:
    virtual TInt32 Begin(TInt32 threadId) = 0;
    virtual TInt32 Run(const TInt32 threadId,const TInt32 runCnt) = 0;
    virtual TInt32 End(TInt32 threadId)    = 0;
    virtual ~IfTask();
public:
    IfTask()
    {
        m_pData = NULL;
    }
    //下面几个可以用，但是经测试，相应时间太长(10ms),是我写错了么？
    void    Init4Event();

    //这里的event是一次性的
    TInt32  Wait4Event(TInt32 timeout=15);
    void    OnNewEvent();
    void    TrySleep(unsigned int ms);
};

}
#endif

#include "TypeDef.h"
#include "SysInc.h"
//Task
#include "TaskCenter.h"
#include "SysMacros.h"
#include "IfLoggerMgr.h"
#include "ExceptionParser.h"
#include <iostream>
#include "WinNetTester.h"
using namespace Zephyr;
using namespace std;
int main()
{
    IfTaskMgr *pTaskMgr = CreateTaskMgr();
    
    CWinNetTester *pNetTester = new CWinNetTester(pTaskMgr);
    cout<<"How many msg do U want send first?";
    int sendMsg;
    cin>>sendMsg;
    char remoteIp[28];
    cin>>remoteIp;
    pNetTester->OnInit();
    
    
    pTaskMgr->AddTask(pNetTester);
    pTaskMgr->StartWorking(4);
    CExceptionParser parser;
    char stop = 'n';
    while(('y' != stop) || ('Y' != stop))
    {
        cout<<"Do U wanna stop?"<<endl;
        cin>>stop;
    }
    pNetTester->OnFianl();
    pTaskMgr->StopWorking();
    ReleaseTaskMgr(pTaskMgr);
    return SUCCESS;
}
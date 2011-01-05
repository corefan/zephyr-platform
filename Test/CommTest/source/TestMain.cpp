#include "TypeDef.h"
#include "SysInc.h"
//Task
#include "TaskCenter.h"
#include "SysMacros.h"
#include "IfLoggerMgr.h"
#include "ExceptionParser.h"
#include "CommTester.h"
#include "LogCenter.h"
#include "CommCenter.h"
#include <iostream>
#include <List>

using namespace Zephyr;
using namespace std;
class CTest
{
public:
    int a;
};
class CTestB : public CTest
{
    //CTestB();
public:

    int b;
};
int main()
{
    

    IfTaskMgr *pTaskMgr = CreateTaskMgr();


    CExceptionParser parser;
    IfLoggerManager *pLogMgr = CreateLogSys(pTaskMgr);
    IfCommunicatorMgr *pMgr = CreateCommMgr(2,pTaskMgr,pLogMgr,NULL);
    
    CCommTester commTest;
    commTest.Init(pMgr,2);
    pTaskMgr->AddTask(&commTest);

    pTaskMgr->StartWorking(4);
    char stop = 'n';
    while(('y' != stop) || ('Y' != stop))
    {
        cout<<"Do U wanna stop?"<<endl;
        cin>>stop;
    }
    pTaskMgr->StopWorking();
    ReleaseTaskMgr(pTaskMgr);
    return SUCCESS;
}
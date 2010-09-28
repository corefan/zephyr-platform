#include "TypeDef.h"
#include "SysInc.h"
//Task
#include "TaskCenter.h"
#include "SysMacros.h"
#include "IfLoggerMgr.h"
#include "ExceptionParser.h"
#include "tplmap.h"
#include <iostream>
#include <stdio.h> 
#include <iostream> 
#include <time.h> 
using namespace Zephyr;
using namespace std;
#define NUM_OF_TEST_TIME 10000
class CMapTest
{
public:
    unsigned int m_key;
};
int main()
{
//     IfTaskMgr *pTaskMgr = CreateTaskMgr();
//     pTaskMgr->StartWorking(4);
//     CExceptionParser parser;
//     
//     char stop = 'n';
//     while(('y' != stop) || ('Y' != stop))
//     {
//         cout<<"Do U wanna stop?"<<endl;
//         cin>>stop;
//     }
//     pTaskMgr->StopWorking();
//     ReleaseTaskMgr(pTaskMgr);
    CMapTest **ppMap = new CMapTest *[NUM_OF_TEST_TIME];
    unsigned int *pRandNr = new unsigned int[NUM_OF_TEST_TIME];
    TplMap<CMapTest,unsigned int> map;
    map.Init(NUM_OF_TEST_TIME+2);
    int testTimes = 0;
    union
    {
        struct 
        {
            unsigned int test:31;
            unsigned int test2:1;
        };
        unsigned int data;
    } t;
    t.test = 1;
    t.test2 = 0;
    do 
    {
        int num = 0;
        srand(time(NULL));
        int randTime = 0;
        while(num<NUM_OF_TEST_TIME)
        {
            unsigned int ran = rand();
            randTime ++;
            ran *= rand();
            CMapTest *pMap = map.GetItemByKey(&ran);
            if (!pMap)
            {
                TInt32 result;
                pMap = map.GetItem(result,&ran);
                if (pMap)
                {
                    pRandNr[num] = ran;
                    ++num;
                }
                else
                {
                    printf("Pool is null?!");
                    int i = 0;
                    i = 100/i;
                }
                //map.CheckTree();
                //++num;
            }
        }
        map.CheckTree();
        TplNode<CMapTest,unsigned int>::Iterator it = map.Begin();
        unsigned int last = 0;
        while(!it.IsNull())
        {
            ppMap[(num-1)] = it.GetItem();
            
            //printf("%d \t",it->m_key);
            if (it->m_key < last)
            {
                printf("Something wrong,it is not in order!");
                int i = 0;
                i = 100/i;
            }
            else
            {
                last = it->m_key;
            }
            if (num <= 0)
            {
                printf("something wrong!");
                int i = 0;
                i = 100/i;
            }
            else
            {
                --num;
            }
            ++it;
            //map.ReleaseItem(ppMap[(num)]);
            //map.CheckTree();
        }
        for (int i =0;i<NUM_OF_TEST_TIME;++i)
        {
            CMapTest *p = map.GetItemByKey(pRandNr + i);
            if (!p)
            {
                printf("Can not find rand nr %u",pRandNr[i]);
            }
            else
            {
                map.ReleaseItem(p);
                map.CheckTree();
            }
        }
        if (0 != num)
        {
            printf("something wrong!");
            int i = 0;
            i = 100/i;
        }
//         it = map.End();
//         last = 0xFFFFFFFF;
//         num = NUM_OF_TEST_TIME;
//         while(!it.IsNull())
//         {
//             //ppMap[(num-1)] = it.GetItem();
// 
//             //printf("%d \t",it->m_key);
//             if (it->m_key > last)
//             {
//                 printf("Something wrong,it is not in order!");
//                 int i = 0;
//                 i = 100/i;
//             }
//             else
//             {
//                 last = it->m_key;
//             }
//             if (num < 0)
//             {
//                 printf("something wrong!");
//                 int i = 0;
//                 i = 100/i;
//             }
//             else
//             {
//                 --num;
//             }
//             --it;
//         }
//         for (int i = 0;i<NUM_OF_TEST_TIME;++i)
//         {
//             map.ReleaseItem(ppMap[i]);
//             map.CheckTree();
//         }
        ++testTimes;
        if (0 == (testTimes%100))
        {
            printf("Test %d times",testTimes);
        }
    } while (1);
    return SUCCESS;
}
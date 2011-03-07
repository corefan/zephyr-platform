// PoolTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "../../Public/tpl/include/TplPool.h"
#include "time.h"
#include "stdlib.h"

class CTestClass
{
public:
    int m_nIdx;
};
using namespace Zephyr;
int main()
{
    CTestClass **ppHolder = new CTestClass*[100000];
    CPool<CTestClass> tPool;
    tPool.InitPool(10000);
    int usedNr=0;
    int runCnt=1;

    __int64 nReleaseTime = 0;
    __int64 nGetTime = 0;
    while(++runCnt)
    {
        if(0==(runCnt%1000))
        {
            srand(time(0));
        }
        //srand(time(0));
        int ran = 0;
        while (0==ran)
        {
            ran = (rand()%10000);
        }
            
        bool bAdd = false;
        if (ran%2)
        {
            bAdd = true;
        }
        if (bAdd)
        {
            //��
            if((ran+usedNr)<100000)
            {
                for(int i=0;i<ran;++i)
                {
                    ppHolder[usedNr]=tPool.GetMem();
                    if (ppHolder[usedNr])
                    {
                        ++usedNr;
                    }
                }
                nGetTime += ran;
            }
        }
        else
        {
            if (ran < usedNr)
            {
                for(int i=0;i<ran;++i)
                {
                    tPool.ReleaseMem(ppHolder[usedNr-1]);
                    --usedNr;
                }
                nReleaseTime += ran;
            }
        }
    }
	return 0;
}


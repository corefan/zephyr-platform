#include<iostream>


#include "../../Public/tpl/include/TplMultiMap.h"
#include "Windows.h"
#include <vector>
#include <map>
using namespace Zephyr;

//#define USE_STL_MAP

class CTestClass
{
public:
    int m_nKey;
    int &GetKey()
    {
        return m_nKey;
    }
   

};

int main(int argc,char *pArgv[])
{


    int nMaxTestTime;
    std::cout<<"Input test time!";
    std::cin>>nMaxTestTime;
    srand(nMaxTestTime);
    CTestClass **ppStore = new CTestClass*[nMaxTestTime];
    for (int i = 0;i<nMaxTestTime;++i)
    {
        ppStore[i] = NULL;
    }
    TplMultiKeyMap<CTestClass,int> m_tMap;
    CPool<TplMultiKeyMapNode<CTestClass,int> > tPool;
    tPool.InitPool(nMaxTestTime);
    m_tMap.Init(&tPool);
    int *pRand = new int[nMaxTestTime];
    std::map<int,CTestClass> tMap;

    for (int i =0;i<nMaxTestTime;++i)
    {
        bool bAgain = true;
#ifdef USE_STL_MAP
        while(bAgain)
#endif
        {
            if (nMaxTestTime > 10000)
            {
                pRand[i] = (rand()%(nMaxTestTime/10000))*10000;
                pRand[i] += rand()%10000;
            }
            else
            {
                pRand[i] = rand();
            }
#ifdef USE_STL_MAP
            std::map<int,CTestClass>::iterator it = tMap.find(pRand[i]);
            if (it == tMap.end())
            {
                CTestClass t;
                tMap[pRand[i]] = t;
                bAgain = false;
            }
#endif
        }
       
    }
    tMap.clear();
    unsigned int uLastTime = 0;
    unsigned int nTestTime = nMaxTestTime;
    unsigned long long nTotalTestTime = 0;
    unsigned long long nLastInfoTime = 1000000;
    unsigned int uLastShowTime = timeGetTime();
    
    while (1)
    {
        if (nMaxTestTime > 10000)
        {
            nTestTime = (rand() % (nMaxTestTime/10000));
            nTestTime *= 10000;
            nTestTime += (rand()%10000);
            //Sleep(15);
        }
        else
        {
            nTestTime = (rand()%nMaxTestTime);
        }
        if (nMaxTestTime > 10000)
        {
            nTestTime = (rand()%(nMaxTestTime/10000))*10000;
            nTestTime += rand()%10000;
        }
        else
        {
            nTestTime = rand() % nMaxTestTime;
        }
        if (nTestTime < (nMaxTestTime>>1))
        {
            nTestTime = (nMaxTestTime>>1);
        }
        nTotalTestTime += nTestTime;
        if (nTotalTestTime > nLastInfoTime)
        {
            unsigned int nTimeNow = timeGetTime();
            unsigned int uGap = nTimeNow - uLastShowTime;
            uLastShowTime = nTimeNow;
            printf("Test %llu times!time is :%u\n\t",nTotalTestTime,uGap);
            nLastInfoTime+=1000000;
        }
        if (nTestTime < (nMaxTestTime>>1))
        {
            nTestTime = (nMaxTestTime>>1);
        }
        for (int i=0;i<nTestTime;++i)
        {
            int n = pRand[((i+nTotalTestTime)%nMaxTestTime)];
#ifdef USE_STL_MAP
            CTestClass tC;
            tC.m_nKey = n;
            tMap[n] = tC;
            //ppStore[i] = &tMap[n];
#else
            ppStore[i] = m_tMap.PrepareItem();
            if (ppStore[i])
            {
                ppStore[i]->m_nKey = n;
                m_tMap.AddInTree(ppStore[i]);
            }
            else
            {
                printf("NULL !");
            }
#endif
        }
        //Sleep(15);
        for (int i=0;i<nTestTime;++i)
        {
#ifdef USE_STL_MAP
            int n = pRand[((i+nTotalTestTime)%nMaxTestTime)];
            tMap.erase(n);
#else
            m_tMap.RemoveFromTreeItem(ppStore[i]);
            int nRtn = m_tMap.ReleaseItem(ppStore[i]);
            if (nRtn < SUCCESS)
            {
                printf("something wrong!");
            }
//             int i1002 = 1002;
//             TplMultiKeyMapNode<CTestClass,int> *pN = (TplMultiKeyMapNode<CTestClass,int> *)m_tMap.GetItemByKey(&i1002);
//             if (pN)
//             {
//                 if (!pN->m_pSiblings)
//                 {
//                     pN = NULL;
//                 }
//             }
            ppStore[i] = NULL;
#endif
        }
        if (m_tMap.GetFreeSize() != nMaxTestTime)
        {
            printf("something wrong!");
        }

        //Sleep(15);
    }
    return 0;
}
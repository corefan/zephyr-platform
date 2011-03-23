#include<iostream>


#include "../../Public/tpl/include/TplMultiMap.h"
#include "Windows.h"
#include <vector>
#include <map>
using namespace Zephyr;

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
    m_tMap.Init(nMaxTestTime);
    unsigned int uLastTime = 0;
    unsigned int nTestTime = nMaxTestTime;
    unsigned long long nTotalTestTime = 0;

    while (1)
    {
        if (nMaxTestTime > 10000)
        {
            nTestTime = (rand() % (nMaxTestTime/10000));
            nTestTime *= 10000;
            nTestTime += (rand()%10000);
        }
        else
        {
            nTestTime = (rand()%nMaxTestTime);
        }
        
        nTotalTestTime += nTestTime;
        if (nTestTime < (nMaxTestTime>>1))
        {
            nTestTime = (nMaxTestTime>>1);
        }
        for (int i=0;i<nTestTime;++i)
        {
            int n = rand()%nTestTime; 
            if(0 == (i%1000))
            {
                unsigned int uTime = timeGetTime();
                if (uTime != uLastTime)
                {
                    uLastTime = uTime;
                    srand(uTime);
                }
            }
            n += rand()%nTestTime;
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
        }
        for (int i=0;i<nTestTime;++i)
        {
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
        }
        if (m_tMap.GetFreeSize() != nMaxTestTime)
        {
            printf("something wrong!");
        }
    }
    return 0;
}
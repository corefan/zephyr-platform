#ifndef __ZEPHYR_PUBLIC_TPL_TPL_POOL_H__
#define __ZEPHYR_PUBLIC_TPL_TPL_POOL_H__
#include "TypeDef.h"
#include "SysMacros.h"

namespace Zephyr
{


template <class ITEM_CLASS>
class ItemClassPool 
{
private:
    TUInt32             m_maxItemNum;
    TUInt32             m_freeItemNum;
    ITEM_CLASS          *m_pItemPool;
    ITEM_CLASS          *m_pFreeHeader;
    ITEM_CLASS          *m_pFreeRear;
    ITEM_CLASS          *m_pUsed;
public:
    ItemClassPool()
    {
        m_maxItemNum = 0;
        m_freeItemNum = 0;       
        m_pItemPool = NULL;
        m_pFreeHeader = NULL;
        m_pFreeRear   = NULL;
        m_pUsed = NULL;
    }
    ~ItemClassPool()
    {
        if (m_pItemPool)
        {
            delete [] m_pItemPool;
        }
    }
    TInt32              Init(TInt32 nrOfMaxItem,TInt32 buffSize)
    {
        try
        {
            m_pItemPool = new ITEM_CLASS[nrOfMaxItem];
        }
        catch (...)
        {
        }
        if (!m_pItemPool)
        {
            return OUT_OF_MEM;
        }
        m_pFreeHeader =m_pItemPool;
        m_pFreeRear = (m_pItemPool);
        m_pFreeHeader->Init();
        for (TUInt32 i=1;i<nrOfMaxItem;i++)
        {
            m_pFreeRear->Attach((m_pItemPool+i));
            m_pFreeRear = (m_pItemPool+i);
            m_pFreeRear->Init();
            int ret = m_pFreeRear->OnCreate(i,buffSize);
            if (ret < SUCCESS)
            {
                delete [] m_pItemPool;
                m_pItemPool = NULL;
                return ret;
            }
        }
        m_maxItemNum = nrOfMaxItem;
        return SUCCESS;
    }
    ITEM_CLASS*        GetItem()
    {
        ITEM_CLASS *pResult = NULL;
        if (m_pFreeHeader)
        {
            pResult = m_pFreeHeader;
            m_pFreeHeader = m_pFreeHeader->GetNext();
            if (!m_pFreeHeader)
            {
                m_pFreeRear = NULL;
            }
            pResult->Detach();
            if (m_pUsed)
            {
                m_pUsed->Attach(pResult);
            }
            else
            {
                m_pUsed = pResult;
            }
            pResult->OnInit();
        }
        return pResult;
    }
    void                ReleaseItem(ITEM_CLASS *pItem)
    {
        //从used列表里删除
        pItem->OnFinal();
        //不负责从原队列里删除，交给使用者.
        pItem->Detach();
        if(m_pFreeRear)
        {
            m_pFreeRear->Attach(pItem);
            m_pFreeRear = pItem;
        }
        else
        {
            m_pFreeHeader = pItem;
            m_pFreeRear   = pItem;
        }
        //pItem->Attach(m_pFree);
    }
    ITEM_CLASS*        GetConectionByIdx(TUInt32 idx)
    {
        if (idx < m_maxItemNum)
        {
            return m_pItemPool + idx;
        }
        return NULL;
    }
};

}


#endif

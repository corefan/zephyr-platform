#ifndef __ZEPHYR_PUBLIC_TPL_TPL_POOL_H__
#define __ZEPHYR_PUBLIC_TPL_TPL_POOL_H__
#include "TypeDef.h"
#include "SysMacros.h"

namespace Zephyr
{
//2级内存池，每个内存对象都保存自己属于哪个内存块CMemBlock，然后释放自己，如果CMemBlock是额外内存，把所有内存都回收了，则释放整个内存块
//所有额外的内存都会自动回收.并且占有非常少的操作
template <class Mem>
class CPool
{
private:
    class CMemBlocks;
    class CPoolMem : public Mem
    {
    public:
        CPoolMem    *m_pMemPoolNext;
        CMemBlocks  *m_pBelongsTo;
    public:
        void Init(CPoolMem *pNext,CMemBlocks *pBelongs2)
        {
            m_pMemPoolNext     = pNext;
            m_pBelongsTo       = pBelongs2;
        }
    };
    class CMemBlock
    {
        DECLARE_CLASS_LIST(CMemBlock)
    private:
        CPoolMem *m_pPools;
        CPoolMem *m_pFree;
        int       m_nSize;
        int       m_nFree;
        bool Belongs2Me(CPoolMem *pPools)
        {
            if ((pPools >= m_pPools) && (pPools < m_pPools+m_nSize))
            {
                return true;
            }
            return false;
        }
    public:
        int GetFreeNr()
        {
            return m_nFree;
        }
        CMemBlock(int size)
        {
            //不能只分配1个吧？！
            if (size == 1)
            {
                size = 2;
            }
            try
            {
                m_pPools = new CPoolMem[size];
            }
            catch (...)
            {
            }
            if (m_pPools)
            {
                for (int i=0;i<size;++i)
                {
                    m_pPools[i].Init((m_pPools+i),this);
                }
                m_pPools[size-1].m_pMemPoolNext = NULL;
            }
            m_pFree = m_pPools;
            m_nFree = size;
            m_nSize = size;

            //m_pNextBlock = NULL;
        }
        ~CMemBlock()
        {
            if (m_pPools)
            {
                delete []m_pPools;
            }
        }
        bool IsEmpty()
        {
            return (m_nFree);
        }
        bool CanRecycle()
        {
            return (m_nFree == m_nSize)
        }
        CPoolMem *GetMem()
        {
            if (m_pFree)
            {
                CPoolMem *pRtn = m_pFree;
                m_pFree = m_pFree->m_pMemPoolNext;
                pRtn->m_pMemPoolNext = NULL;
                --m_nFree;
                return pRtn;
            }
            return NULL;
        }
        bool      ReleaseMem(CPoolMem *pRel)
        {
            if(Belongs2Me(pRel))
            {
                pRel->m_pMemPoolNext = m_pFree;
                m_pFree = pRel;
                ++m_nFree;
                return true;
            }
            return false;
        }
    };
    CMemBlock *m_pMemBlocksHeader;
    CMemBlock *m_pMemBlocksRear;
    CMemBlock *m_pMainBlocks;
    int        m_nInitSize;
    int        m_nFreeNr;
public:
    int GetFreeNr()
    {
        return m_nFreeNr;
    }
    int InitPool(unsigned int size)
    {
        try 
        {
            m_pMemBlocksHeader = new CMemBlock(size);
        }
        catch(...)
        {

        }
        if (!m_pMemBlocksHeader)
        {
            return -1;
        }
        m_pMemBlocksRear = m_pMemBlocksHeader;
        m_pMainBlocks = m_pMemBlocksRear;
        m_nInitSize = size;
        m_nFreeNr   = size;
    }
    bool IsNotMainBlock(CMemBlock* pBlock)
    {
        if (pBlock != m_pMemBlocksHeader)
        {
            return true;
        }
        return false;
    }
    void OnBlockUsed(CMemBlock *pBlocks)
    {
        if (pBlocks->IsEmpty())
        {
            if (pBlocks == m_pMemBlocksHeader)
            {
                m_pMemBlocksHeader = m_pMemBlocksHeader->GetNext();
            }
            if (pBlocks == m_pMemBlocksRear)
            {
                m_pMemBlocksRear = m_pMemBlocksRear->GetPrev();
            }
            pBlocks->Detach();
        }
    }
    //之时是否要delete 这个块所属的Blocks
    void OnBlockRecycled(CMemBlock *pBlock,bool bIsEmpty)
    {
        if (bIsEmpty)
        {
            if (m_pMemBlocksRear)
            {
                m_pMemBlocksRear->Attach(pBlock)
            }
            else
            {
                m_pMemBlocksHeader = pBlock;
                m_pMemBlocksRear   = pBlock;
            }
        }
        else
        {
            //不是主内存块
            if (IsNotMainBlock(pBlock))
            {
                //可以释放
                if (pBlock->CanRecycle())
                {
                    //而且也有很多空闲了
                    int remains = m_nFreeNr-pBlock->GetFreeNr();
                    
                    if ((remains) > (m_nInitSize>>3))
                    {
                        if (m_pMemBlocksHeader == pBlock)
                        {
                            m_pMemBlocksHeader = pBlock->GetNext();
                        }
                        if (m_pMemBlocksRear == pBlock)
                        {
                            m_pMemBlocksRear = pBlock->GetPrev();
                        }
                        pBlock->Detach();
                        
                        delete pBlock;
                        m_nFreeNr = remains;
                    }
                }
            }
        }
    }
    Mem *GetMem()
    {
        //CMemBlock *pBlock = m_pMemBlocksHeader;
        if (m_pMemBlocksHeader)
        {
            CPoolMem *pRtn = m_pMemBlocksHeader->GetMem();
            if (pRtn)
            {
                OnBlockUsed(m_pMemBlocksHeader);
                --m_nFreeNr;
                return pRtn;
            }
        }
        CMemBlock *pBlock;
        try
        {
            pBlock = new CMemBlock((m_nInitSize>>1));
        }
        catch (...)
        {
        }
        if (pBlock)
        {
            if (m_pMemBlocksRear)
            {
                m_pMemBlocksRear->Attach(pBlock)
            }
            else
            {
                m_pMemBlocksHeader = pBlock;
                m_pMemBlocksRear   = pBlock;
            }
            m_nFreeNr += pBlock->GetFreeNr();
            //肯定成功
            --m_nFreeNr;
            return pBlock->GetMem();
        }
        return NULL;
    }
    bool ReleaseMem(Mem *pMem)
    {
        CPoolMem *p = (CPoolMem*)pMem;
        CMemBlock *pBlock = p->m_pBelongsTo;
        bool bIsEmpty = pBlock->IsEmpty();
        if(pBlock->ReleaseMem(p))
        {
            ++m_nFreeNr;
        }
        OnBlockRecycled(p,bIsEmpty);
    }
};


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
        m_freeItemNum = nrOfMaxItem;
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
            pResult->Attach(m_pUsed);
            m_pUsed = pResult;
            pResult->OnInit();
            --m_freeItemNum;
        }
        return pResult;
    }
    void                ReleaseItem(ITEM_CLASS *pItem)
    {
        //从used列表里删除
        if (!pItem->IsActived())
        {
            return;
        }
        pItem->OnFinal();
        //不负责从原队列里删除，交给使用者.
        if (m_pUsed == pItem)
        {
            m_pUsed = pItem->GetNext();
        }
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
        ++m_freeItemNum;
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

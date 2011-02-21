#ifndef __ZEPHYR_PUBLIC_TPL_TPL_POOL_H__
#define __ZEPHYR_PUBLIC_TPL_TPL_POOL_H__
#include "TypeDef.h"
#include "SysMacros.h"
#include "TplList.h"
namespace Zephyr
{
//2级内存池，每个内存对象都保存自己属于哪个内存块CMemBlock，然后释放自己，如果CMemBlock是额外内存，把所有内存都回收了，则释放整个内存块
//所有额外的内存都会自动回收.并且占有非常少的操作
template <class Mem>
class CPool
{
private:
    class CMemBlock;
    class CPoolMem : public Mem
    {
    public:
        CPoolMem    *m_pMemPoolNext;
        CMemBlock  *m_pBelongsTo;
    public:
        void Init(CPoolMem *pNext,CMemBlock *pBelongs2)
        {
            m_pMemPoolNext     = pNext;
            m_pBelongsTo       = pBelongs2;
        }
    };
    class CMemBlock
    {
        //DECLARE_CLASS_LIST(CMemBlock)
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
        int Init(int size)
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
            else
            {
                return OUT_OF_MEM;
            }
            m_pFree = m_pPools;
            m_nFree = size;
            m_nSize = size;
            return SUCCESS;
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
            return (m_nFree == m_nSize);
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
    CList<CMemBlock> m_tUsingMemBlocks;
    CList<CMemBlock> m_tFullMemBlocks;
    CListNode<CMemBlock> *m_pMainBlock;
    int        m_nInitSize;
    int        m_nFreeNr;
public:
    int GetFreeNr()
    {
        return m_nFreeNr;
    }
    int InitPool(unsigned int size)
    {
        CListNode<CMemBlock> *pBlock = NULL;
        try 
        {
            pBlock = new CListNode<CMemBlock>;
        }
        catch(...)
        {

        }
        if (!pBlock)
        {
            return OUT_OF_MEM;
        }
        m_tUsingMemBlocks.push_front(pBlock);
        int nRet = pBlock->Init(size);
        if (nRet < 0)
        {
            delete pBlock;
            return nRet;
        }
        m_pMainBlock = pBlock;
        m_nInitSize = size;
        m_nFreeNr   = size;
    }
    bool IsNotMainBlock(CMemBlock* pBlock)
    {
        if (pBlock != m_pMainBlock)
        {
            return true;
        }
        return false;
    }
    void OnBlockUsed(CListNode<CMemBlock> *pBlocks)
    {
        if (pBlocks->IsEmpty())
        {
            m_tUsingMemBlocks.HandleOver(&m_tFullMemBlocks,pBlocks);
        }
    }
    //之时是否要delete 这个块所属的Blocks
    void OnBlockRecycled(CMemBlock *pBlock,bool bIsEmpty)
    {
        
    }
    Mem *GetMem()
    {
        CListNode<CMemBlock> *pBlock = m_tUsingMemBlocks.header();
        if (pBlock)
        {
            CPoolMem *pRtn = pBlock->GetMem();
            if (pRtn) //这个是必须的
            {
                OnBlockUsed(pBlock);
                --m_nFreeNr;
                return pRtn;
            }
            else //呃回收一下？
            {
                OnBlockUsed(pBlock); 
            }
        }
        try
        {
            pBlock = new CListNode<CMemBlock>();
        }
        catch (...)
        {
        }
        if (pBlock)
        {
            if (pBlock->Init(m_nInitSize>>1) < SUCCESS)
            {
                delete pBlock;
                return NULL;
            }
        }
        if (pBlock)
        {
            m_tUsingMemBlocks.push_front(pBlock);
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
        OnBlockRecycled(pBlock,bIsEmpty);
        return true;
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

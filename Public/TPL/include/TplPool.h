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
    TUInt32             m_maxConnectionNum;
    TUInt32             m_freeConnectionNum;
    ITEM_CLASS          *m_pConnectionPool;
    ITEM_CLASS          *m_pFreeHeader;
    ITEM_CLASS          *m_pFreeRear;
    ITEM_CLASS          *m_pUsed;
public:
    ItemClassPool()
    {
        m_maxConnectionNum = 0;
        m_freeConnectionNum = 0;       
        m_pConnectionPool = NULL;
        m_pFreeHeader = NULL;
        m_pFreeRear   = NULL;
        m_pUsed = NULL;
    }
    ~ItemClassPool()
    {
        if (m_pConnectionPool)
        {
            delete [] m_pConnectionPool;
        }
    }
    TInt32              Init(TInt32 nrOfMaxConnection,TInt32 buffSize)
    {
        try
        {
            m_pConnectionPool = new ITEM_CLASS[nrOfMaxConnection];
        }
        catch (...)
        {
        }
        if (!m_pConnectionPool)
        {
            return OUT_OF_MEM;
        }
        m_pFreeHeader =m_pConnectionPool;
        m_pFreeRear = (m_pConnectionPool);
        m_pFreeHeader->OnInit();
        for (TUInt32 i=1;i<nrOfMaxConnection;i++)
        {
            m_pFreeRear->Attach((m_pConnectionPool+i));
            m_pFreeRear = (m_pConnectionPool+i);
            m_pFreeRear->OnInit();
            int ret = m_pFreeRear->OnCreate(i,buffSize);
            if (ret < SUCCESS)
            {
                delete [] m_pConnectionPool;
                m_pConnectionPool = NULL;
                return ret;
            }
        }
        m_maxConnectionNum = nrOfMaxConnection;
        return SUCCESS;
    }
    CConnection*        GetConnection()
    {
        CConnection *pResult = NULL;
        if (m_pFreeHeader)
        {
            pResult = m_pFreeHeader;
            m_pFreeHeader = m_pFreeHeader->GetNext();
            if (!m_pFreeHeader)
            {
                m_pFreeRear = NULL;
            }
            pResult->Detach();
            m_pUsed->Attach(pResult);
            pResult->OnInit();
        }
        return pResult;
    }
    void                ReleaseConnection(CConnection *pConnection)
    {
        //从used列表里删除
        pConnection->OnFinal();
        //不负责从原队列里删除，交给使用者.
        pConnection->Detach();
        if(m_pFreeRear)
        {
            m_pFreeRear->Attach(pConnection);
            m_pFreeRear = pConnection;
        }
        else
        {
            m_pFreeHeader = pConnection;
            m_pFreeRear   = pConnection;
        }
        //pConnection->Attach(m_pFree);
    }
    CConnection*        GetConectionByIdx(TUInt32 idx)
    {
        if (idx < m_maxConnectionNum)
        {
            return m_pConnectionPool + idx;
        }
        return NULL;
    }
};

}


#endif

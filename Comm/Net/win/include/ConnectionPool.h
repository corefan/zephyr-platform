#ifndef __ZEPHYR_COMM_CONNECTION_POOL_H__
#define __ZEPHYR_COMM_CONNECTION_POOL_H__
#include "TypeDef.h"
#include "SysMacros.h"
#include "Connection.h"

namespace Zephyr
{
class CConnectionPool
{
private:
    TUInt32             m_maxConnectionNum;
    TUInt32             m_freeConnectionNum;         
    CConnection*        m_pConnectionPool;
    CConnection*        m_pFree;
    CConnection*        m_pUsed;
public:
    CConnectionPool()
    {
        m_maxConnectionNum = 0;
        m_freeConnectionNum = 0;       
        m_pConnectionPool = NULL;
        m_pFree = NULL;
        m_pUsed = NULL;
    }
    ~CConnectionPool()
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
            m_pConnectionPool = new CConnection[nrOfMaxConnection];
        }
        catch (...)
        {
        }
        if (!m_pConnectionPool)
        {
            return OUT_OF_MEM;
        }
        for (TUInt32 i=0;i<nrOfMaxConnection;i++)
        {
            m_pConnectionPool[i].Attach(m_pFree);
            m_pConnectionPool[i].OnInit();
            int ret = m_pConnectionPool[i].OnCreate(i,buffSize);
            if (ret < SUCCESS)
            {
                delete [] m_pConnectionPool;
                m_pConnectionPool = NULL;
                return ret;
            }
            m_pFree = (m_pConnectionPool + i);
        }
        return SUCCESS;
    }
    CConnection*        GetConnection()
    {
        CConnection *pResult = NULL;
        if (m_pFree)
        {
            pResult = m_pFree;
            m_pFree = m_pFree->GetNext();
            pResult->Detach();
            pResult->OnInit();
        }
        return pResult;
    }
    
    void                ReleaseConnection(CConnection *pConnection)
    {
        pConnection->OnFinal();
        //不负责从原队列里删除，交给使用者.
        //pConnection->Detach();
        pConnection->Attach(m_pFree);
        m_pFree = pConnection;
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

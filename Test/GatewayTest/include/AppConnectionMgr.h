#ifndef __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_MGR_H__
#define __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_MGR_H__

#include "IfNetApp.h"
#include "AppConnection.h"
#include "TypeDef.h"
#include "Public/tpl/include/TplList.h"
#include "Public/tpl/include/TplPool.h"
#include "Public/include/Clock.h"

using namespace Zephyr;

class CAppConnectionMgr : public IfListenerCallBack
{
private:
    CPool<CSafeListNode<CAppConnection> > m_tPool;
    CList<CAppConnection> m_tWaitingConnectionList;
    CList<CAppConnection> m_tUsingList;
    CList<CAppConnection> m_tTryingList;
    CList<CAppConnection> m_tDisconnectedList;
    CClock                m_tClock;
public:
    CClock *GetClock()
    {
        return &m_tClock;
    }
    void UpdateClock()
    {
        m_tClock.Update();
    }
    virtual IfConnectionCallBack *OnNewConnection(CConPair *pPair);
    TInt32  Init(TInt32 maxConnectionNr);
    CAppConnection *GetConnection();
    int GetConnectionNr()
    {
        return m_tUsingList.size();
    }
    void ReleaseConnection(CAppConnection *pConnection);
    CList<CAppConnection> *GetUsingList()
    {
        return &m_tUsingList;
    }

    void AddToUsedList(CAppConnection *pItem)
    {
        CSafeListNode<CAppConnection> *pSafeNode = (CSafeListNode<CAppConnection> *)pItem;
        pSafeNode->Attach2(&m_tUsingList);
    }
    void AddToTryingList(CAppConnection *pItem)
    {
        CSafeListNode<CAppConnection> *pSafeNode = (CSafeListNode<CAppConnection> *)pItem;
        pSafeNode->Attach2(&m_tTryingList);
    }
    void AddToDisconnectedList(CAppConnection *pItem)
    {
        CSafeListNode<CAppConnection> *pSafeNode = (CSafeListNode<CAppConnection> *)pItem;
        pSafeNode->Attach2(&m_tDisconnectedList);
    }
};

#endif

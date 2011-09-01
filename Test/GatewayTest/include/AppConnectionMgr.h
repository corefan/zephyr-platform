#ifndef __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_MGR_H__
#define __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_MGR_H__

#include "IfNetApp.h"
#include "AppConnection.h"
#include "TypeDef.h"
#include "Public/tpl/include/TplList.h"
#include "Public/tpl/include/TplPool.h"
using namespace Zephyr;

class CAppConnectionMgr : public IfListenerCallBack
{
private:
    CPool<CSafeListNode<CAppConnection> > m_tPool;
    CList<CAppConnection> m_tUsingList;
public:

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
};

#endif

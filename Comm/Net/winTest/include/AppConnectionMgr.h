#ifndef __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_MGR_H__
#define __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_MGR_H__

#include "IfNetApp.h"
#include "AppConnection.h"
#include "TypeDef.h"

using namespace Zephyr;

class CAppConnectionPool : public IfListenerCallBack
{
private:
    CAppConnection  *m_pConnectionPool;
    CAppConnection  *m_pFree;
    CAppConnection  *m_pUsed:
    TInt32          m_connectionNr;
public:
    virtual IfConnectionCallBack *OnNewConnection(CConPair *pPair);
    
    CAppConnection *GetConnection();
    void ReleaseConnection(CAppConnection *pConnection);
};

#endif

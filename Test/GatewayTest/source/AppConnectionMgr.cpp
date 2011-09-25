#include "AppConnectionMgr.h"


using namespace Zephyr;

IfConnectionCallBack *CAppConnectionMgr::OnNewConnection(CConPair *pPair)
{
    return GetConnection();
}
TInt32  CAppConnectionMgr::Init(TInt32 maxConnectionNr)
{
    m_tClock.Update();
    return m_tPool.InitPool(maxConnectionNr);
}
CAppConnection *CAppConnectionMgr::GetConnection()
{
    CSafeListNode<CAppConnection> *pResult = NULL;
    pResult = m_tPool.GetMem();
    pResult->OnInit();
    pResult->Attach2(&m_tUsingList);
    pResult->SetMgr(this);
    
    return pResult;
}

void CAppConnectionMgr::ReleaseConnection(CAppConnection *pConnection)
{
    CSafeListNode<CAppConnection> *pResult = (CSafeListNode<CAppConnection> *)pConnection;
    pResult->OnFinal();
    pResult->Detach();
    m_tPool.ReleaseMem(pResult);
    //pConnection->Attach(m_pFree);
    
}
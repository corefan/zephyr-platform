#include "AppConnectionMgr.h"

IfConnectionCallBack *CAppConnectionMgr::OnNewConnection(CConPair *pPair)
{
    return GetConnection();
}
TInt32  CAppConnectionMgr::Init(TInt32 maxConnectionNr)
{
    NEW(m_pConnectionPool,CAppConnection,maxConnectionNr);
    if (!m_pConnectionPool)
    {
        return OUT_OF_MEM;
    }
    for (int i=0;i<maxConnectionNr;++i)
    {
        m_pConnectionPool[i].Attach(m_pFree);
        m_pFree = m_pConnectionPool + i;
    }
    m_pUsed = NULL;
    return SUCCESS;
}
CAppConnection *CAppConnectionMgr::GetConnection()
{
    CAppConnection *pResult = NULL;
    if (m_pFree)
    {
        pResult = m_pFree;
        m_pFree = m_pFree->Detach(NULL);
        pResult->OnInit();
    }
    return pResult;
}
void CAppConnectionMgr::ReleaseConnection(CAppConnection *pConnection)
{
    pConnection->OnFinal();
}
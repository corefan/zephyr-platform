#include "../include/DBAuthenticateTrans.h"
#include "../include/AuthenticateService.h"
#include "SyStem/DBLib/DBInterfaces.h"
#include <string.h>
#include "System/DBLib/MySQLRecordSet.h"
namespace erp_platform
{



void CDBAuthenticateTrans::Init(CAuthenticateData *pData,CAuthenticateService *pService)
{
    m_nContinue = 1;
    memcpy(&m_unAllData.m_tAuthenticateData,pData,sizeof(CAuthenticateData));
    m_pService = pService;
}


CDBAuthenticateTrans::CDBAuthenticateTrans()
{
    m_nContinue = 0;
    memset(&m_unAllData,0,sizeof(m_unAllData));
    m_pService = NULL;
    m_nResult = SUCCESS;
}

bool CDBAuthenticateTrans::OnExecute(void * pConnection)
{
    TChar szBuffer[200];
    IDBConnection *pMysqlCon = (IDBConnection *)pConnection;
    sprintf(szBuffer,"SELECT * FROM Logic.ACCOUNT WHERE m_szName='%s'",m_unAllData.m_tAuthenticateData.m_szName);
    if(pMysqlCon->ExecuteSQL(szBuffer) != DBERR_SUCCEED)
    {
        return false;
    }

    CMySQLRecordSet tRecordSet;

    int nRet = pMysqlCon->GetResults(&tRecordSet);
    if(nRet == DBERR_SUCCEED)
    {
        int itemNr = tRecordSet.GetRecordCount();
        tRecordSet.GetField(0);
        tRecordSet.MoveNext();
    }
    return true;
}

void CDBAuthenticateTrans::OnFinish()
{
    m_pService->OnDbFinished(this);
}

}
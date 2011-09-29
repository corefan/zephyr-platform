#include "../include/AuthenticateService.h"

namespace Zephyr
{
CAuthenticateService::CAuthenticateService()
{
    m_pDbMgr        = NULL;
    m_pIfOrb        = NULL;
    m_pIfTaskMgr    = NULL;
    m_pLoggerMgr    = NULL;
    m_pLogger       = NULL;
}

CAuthenticateService::~CAuthenticateService()
{
    if (m_pDbMgr)
    {
        ReleaseMgr(m_pDbMgr);
        m_pDbMgr = NULL;
    }
    if (m_pLogger)
    {
        m_pLoggerMgr->ReleaseLogger(m_pLogger);
        m_pLogger = NULL;
    }
}

TInt32 CAuthenticateService::Authenticate(TLV<TUInt16,TUInt16> tAuthenticateData)
{
    return SUCCESS;
}




CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr)
{
    CAuthenticateService *pAuthenticateScrvice=NULL;
    try
    {
        pAuthenticateScrvice = new CAuthenticateService();
    }
    catch (...)
    {
        return NULL;
    }
    

    return pAuthenticateScrvice;
}

TInt32 ReleaseService(CService* pService)
{

    return SUCCESS;
}

//�ڳ�ʼ����ʱ��ᱻ��.
TInt32 CAuthenticateService::OnInit()
{
    //m_pDbMgr = GetMysqlWorkThreadMgr()
    return SUCCESS;
}
//�����ǻص�.
TInt32 CAuthenticateService::OnFinal()
{
    return SUCCESS;
}


}
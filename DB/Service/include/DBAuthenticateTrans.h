#ifndef __ZEPHYR_DB_AUTHENTICATE_TRANS_H__
#define __ZEPHYR_DB_AUTHENTICATE_TRANS_H__

#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"
#include "System/DBLib/IfDBLib.h"
#include "AuthenticateData.h"
using namespace DBLib;

namespace Zephyr
{

class CDBAuthenticateTrans : public CDBTransaction
{
public:
    //��˭�����.
    volatile TUInt32 m_nContinue;
    CDoid  m_tSrcDoid;
    union UnData
    {
        //��������
        CAuthenticateData m_tAuthenticateData;
        //���
        CAuthorityData    m_tAuthorityData;
    };
    UnData m_unAllData;
    CDoid &GetKey()
    {
        return m_tSrcDoid;
    }
public:
    void Init(CAuthenticateData *pData);
    void OnDisconnected()
    {
        m_nContinue = 0;
    }
    TBOOL IsContinue()
    {
        if (m_nContinue)
        {
            return TRUE;
        }
        return FALSE;
    }
    CDBAuthenticateTrans();
    virtual bool OnExecute(void * pConnection);
    virtual void OnFinish();
};
}

#endif

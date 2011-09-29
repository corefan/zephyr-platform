#ifndef __ZEPHYR_AUTHENTICATE_DATA_H__
#define __ZEPHYR_AUTHENTICATE_DATA_H__

#include "Public/include/TypeDef.h"

namespace Zephyr
{

#define MAX_ACCOUNT_NAME_LENGTH 32
#define MAX_ACCOUNT_PWD_LENGTH 32
#define MAX_DYNAMIC_PWD_DATA_LENGTH 32

struct CAuthenticateData
{
public:
    TChar m_szName[MAX_ACCOUNT_NAME_LENGTH];
    TChar m_szPwd[MAX_ACCOUNT_PWD_LENGTH];
    TUInt32 m_uLoginIp;
};

struct CAuthorityData
{
public:
    TUInt64 m_uAccountId; //64λId
    TUInt32 m_uProirty;   //Ȩ��
    TUInt32 m_uLastLoginTime; //�ϴε�¼ʱ��
    TUInt32 m_uLastLoginIp;   //�ϴε�¼��ַ
    TUInt32 m_uBanTime;       //��ͣʱ��
};

}

#endif

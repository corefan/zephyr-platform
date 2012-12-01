#include "../include/TJobInfosGetLength.h"
#include "Public/include/TypeMarshaller.h"
using namespace erp_platform;
TInt32 GetLength(const TJobInfos *_pT)
{
    return GetLength(*_pT);
}
TInt32 GetLength(const TJobInfos &_rValue)
{
return GetLength(_rValue.m_uMenuId)+GetLength(_rValue.m_uLvl)+GetLength(_rValue.m_uFatherMenuId)+GetLength(_rValue.m_pszNames);
}

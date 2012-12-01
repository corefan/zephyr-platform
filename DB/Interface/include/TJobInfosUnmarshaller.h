#ifndef __TJOBINFOS_UNMARSHALLER_H__
#define __TJOBINFOS_UNMARSHALLER_H__
#include "Public/include/Typedef.h"
#include "TJobInfosGetLength.h"
#include "../Interface/JobInfo.h"
using namespace erp_platform;
TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,TJobInfos &_rValue);
TInt32 Unmarshall(TUChar *_pBuff,TInt32 _nLength,TJobInfos *&_pT);
#endif

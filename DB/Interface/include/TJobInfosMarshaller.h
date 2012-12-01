#ifndef __TJOBINFOS_MARSHALLER_H__
#define __TJOBINFOS_MARSHALLER_H__
#include "Public/include/Typedef.h"
#include "TJobInfosGetLength.h"
#include "../Interface/JobInfo.h"
using namespace erp_platform;
TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const TJobInfos &_rValue);
TInt32 Marshall(TUChar *_pBuff,TInt32 _nLength,const TJobInfos *_pT);
#endif

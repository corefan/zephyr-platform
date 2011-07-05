#ifndef __ZEPHYR_IF_GATEWAY_H__
#define __ZEPHYR_IF_GATEWAY_H__

#include "Typedef.h"

namespace Zephyr
{
class IfGatewaySvc
{
public:
    //ͬ����Ϣ.
    TInt32 Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV);
    //ע�����
    TInt32 RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName);
    //ע������
    TInt32 UnRegisterService(TUInt32 uServiceId);
    //���͹㲥������Ϣ
    TInt32 BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName);
};
}




#endif

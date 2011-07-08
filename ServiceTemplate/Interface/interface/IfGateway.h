#ifndef __ZEPHYR_IF_GATEWAY_H__
#define __ZEPHYR_IF_GATEWAY_H__

#include "Public/include/Typedef.h"

namespace Zephyr
{
class IfGatewaySvc
{
public:
    //ͬ����Ϣ.
    virtual TInt32 Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV) = 0;
    //ע�����
    virtual TInt32 RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName) = 0;
    //ע������
    virtual TInt32 UnRegisterService(TUInt32 uServiceId) = 0;
    //���͹㲥������Ϣ
    virtual TInt32 BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName) = 0;
};
}




#endif

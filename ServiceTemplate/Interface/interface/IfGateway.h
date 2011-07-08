#ifndef __ZEPHYR_IF_GATEWAY_H__
#define __ZEPHYR_IF_GATEWAY_H__

#include "Public/include/Typedef.h"

namespace Zephyr
{
class IfGatewaySvc
{
public:
    //同步消息.
    virtual TInt32 Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV) = 0;
    //注册服务
    virtual TInt32 RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName) = 0;
    //注销服务
    virtual TInt32 UnRegisterService(TUInt32 uServiceId) = 0;
    //发送广播聊天信息
    virtual TInt32 BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName) = 0;
};
}




#endif

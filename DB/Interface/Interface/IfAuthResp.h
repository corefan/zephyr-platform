#ifndef __ZEPHYR_DB_IF_AUTH_RESP_H__
#define __ZEPHYR_DB_IF_AUTH_RESP_H__

#include "Public/include/TypeDef.h"

namespace Zephyr
{


class IfAuthResp
{
public:
    //第一个参数是结果，第二个是鉴权结果
    virtual TInt32 RespAuthenticate(TInt32 nResult,TLV<TUInt16,TUInt16> tAuthorityData) = 0;

    //这个就是为了处理确认连接中断的，这样AS就会在数据库读取完毕之后，扔掉对应连接的结果.
    virtual TInt32 ConfirmDisconneted(CDoid tMyDoid) = 0;
};
}
#endif

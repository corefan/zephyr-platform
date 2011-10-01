#ifndef __ZEPHYR_DB_IF_AUTH_RESP_H__
#define __ZEPHYR_DB_IF_AUTH_RESP_H__

#include "Public/include/TypeDef.h"

namespace Zephyr
{


class IfAuthResp
{
public:
    //��һ�������ǽ�����ڶ����Ǽ�Ȩ���
    virtual TInt32 RespAuthenticate(TInt32 nResult,TLV<TUInt16,TUInt16> tAuthorityData) = 0;

    //�������Ϊ�˴���ȷ�������жϵģ�����AS�ͻ������ݿ��ȡ���֮���ӵ���Ӧ���ӵĽ��.
    virtual TInt32 ConfirmDisconneted(CDoid tMyDoid) = 0;
};
}
#endif

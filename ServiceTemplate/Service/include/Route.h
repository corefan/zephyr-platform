#ifndef __ZEPHYR_GATEWAY_ROUTE_H__
#define __ZEPHYR_GATEWAY_ROUTE_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"

namespace Zephyr
{

class CRoute
{
public:
    TUInt32 m_uKey;
    //���ܻ�ע�Ჿ�ֹ���.
    TUInt32 m_uIdBegin;
    TUInt32 m_uIdEnd;
    //
    CDoid   m_tRouteTo;
};

}
#endif

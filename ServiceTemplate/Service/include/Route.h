#ifndef __ZEPHYR_GATEWAY_ROUTE_H__
#define __ZEPHYR_GATEWAY_ROUTE_H__
#include "Public/include/TypeDef.h"
#include "Public/include/Doid.h"

namespace Zephyr
{

class CRoute
{
public:
    TUInt32 &GetKey()
    {
        return m_uKey;
    }
    TUInt32 m_uKey;
    //���ܻ������������û��ϵ.
    //���ܻ�ע�Ჿ�ֹ���.
    TUInt32 m_uIdBegin;
    TUInt32 m_uIdEnd;
    TUInt32 m_uPriority; //Ȩ�أ�����������ؾ����ʱ��ʹ��
    CDoid   m_tRouteTo;
    CRoute()
    {
        m_uKey = 0;
        m_uIdBegin = 0;
        m_uIdEnd = 0;
        m_uPriority = 0;
    }
    //�����߱�֤��uServiceId��ͬ������rNew��idBegin\idEnd���Ϸ�
    TBOOL Merge(CRoute &rNew)
    {
        if (m_tRouteTo == rNew.m_tRouteTo)
        {
            if (m_uIdBegin > rNew.m_uIdBegin)
            {
                if (m_uIdEnd <= rNew.m_uIdBegin)
                {
                    if (m_uIdEnd < rNew.m_uIdEnd)
                    {
                        m_uIdEnd = rNew.m_uIdEnd;
                    }
                    //ʹ���µ�Ȩ��
                    m_uPriority = rNew.m_uPriority;
                    return TRUE;
                    //�����ں�
                }
            }
            else if (m_uIdBegin >= rNew.m_uIdEnd)
            {
                //�����ں�
                m_uIdBegin = rNew.m_uIdBegin;
                if (m_uIdEnd < rNew.m_uIdEnd)
                {
                    m_uIdEnd = rNew.m_uIdEnd;
                }
                m_uPriority = rNew.m_uPriority;
                return TRUE;
            }
        }
        return FALSE;
    }
};

}
#endif

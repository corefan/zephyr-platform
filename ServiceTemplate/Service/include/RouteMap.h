#ifndef __ZEPHYR_GATEWAY_ROUTE_MAP_H__
#define __ZEPHYR_GATEWAY_ROUTE_MAP_H__
#include "Route.h"
#include "Public/tpl/include/TplMultiMap.h"
#include "Public/tpl/include/TplPool.h"
#include "Public/Interface/Platform/include/IfLogger.h"
#include "Public/include/Doid.h"

namespace Zephyr
{

class CRouteMap
{
private:
    TplMultiKeyMap<CRoute,TUInt32>             m_tServiceRoute;

    IfLogger                                   *m_pLogger;
public:
    CRouteMap()
    {
        m_pLogger = NULL;
    }
    void Init(CPool<TplMultiKeyMapNode<CRoute,TUInt32> > *pPool,IfLogger *pLogger)
    {
        m_pLogger = pLogger;
        m_tServiceRoute.Init(pPool);
    }
    IfLogger *GetLogger()
    {
        return m_pLogger;
    }
    TInt32 AddRoute(CDoid *pDoid,CDoid *pRegister,TUInt32 uSrvId,TUInt32 uBegin,TUInt32 uEnd,TUInt32 uPriority=0);
    CDoid *FindService(TUInt32);
    void OnFinal();
};

}
#endif

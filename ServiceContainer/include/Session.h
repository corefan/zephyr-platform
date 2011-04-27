#include "./Skeleton.h"
#include "../../Public/include/TypeDef.h"
#include "../../Public/include/SysMacros.h"
#include "./Service.h"

namespace Zephyr
{

class CSession : public CObject
{
private:
    CService    *m_pService;
public:
    CSession();
    virtual ~CSession();
    TInt32 Init(CService *pService);
};

}
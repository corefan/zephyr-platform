#include "NetCenter.h"

#include "ConnectionMgr.h"
namespace Zephyr
{

IfNet *CreateNet(IfTaskMgr *pTaskMgr,IfParserFactory *pParserFactory,IfCryptorFactory *pCryptorFactory,TInt32 maxConnectionNr,TUInt32 buffSize)
{
    CConnectionMgr *pConnectionMgr = new CConnectionMgr();
    int ret = pConnectionMgr->Init(maxConnectionNr,pTaskMgr,pParserFactory,pCryptorFactory,buffSize);
    if (ret < SUCCESS)
    {
        delete pConnectionMgr;
        pConnectionMgr = NULL;
    }
    return pConnectionMgr;
}

};
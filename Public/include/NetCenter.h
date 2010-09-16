#ifndef __ZEPHYR_PUBLIC_NET_CENTER_H__
#define __ZEPHYR_PUBLIC_NET_CENTER_H__
#include "IfCryptorFactory.h"
#include "IfParserFactory.h"
#include "IfTaskMgr.h"
#include "TypeDef.h"
#include "IfNet.h"
namespace Zephyr
{
IfNet *CreateNet(IfTaskMgr *pTaskMgr,IfParserFactory *pParserFactory,IfCryptorFactory *pCryptorFactory=NULL,TInt32 maxConnectionNr = 64,TUInt32 buffSize = (256*1024));
};

#endif;
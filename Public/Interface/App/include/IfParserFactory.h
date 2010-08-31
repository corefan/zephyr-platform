/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfTask.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 协议解析器工厂接口
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __IF_PARSER_FACTORY_H__
#define __IF_PARSER_FACTORY_H__

#include "TypeDef.h"
#include "IfParser.h"
namespace Zephyr
{

class IfParserFactory
{
public:
    virtual IfParser *GetParser(TUInt32 myIp,TUInt32 remoteIp,TUInt16 myPort,TUInt16 remotePort,TUInt32 connectionIdx) = 0;
    virtual void ReleaseParser(IfParser *pParser) = 0;
};

}
 #endif
 
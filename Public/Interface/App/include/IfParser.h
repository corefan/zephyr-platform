/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfTask.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: 协议解析器接口
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/


#ifndef __ZEPHYR_IF_PARSER_H__
#define __ZEPHYR_IF_PARSER_H__

#include "TypeDef.h"
//#include "Message.h"

namespace Zephyr
{

class IfParser
{
public:
    virtual TInt32  GetMaxHeaderLength() = 0;
    virtual TInt32  GetMinHeaderLength() = 0;
    virtual TInt32  OnRecv(TUChar *pNetData,TInt32 dataLen,TInt32& bodyLength,TInt32& neededBodyLength) = 0;
    virtual TInt32  ParseNetData(TUChar *pNetData,TInt32 length, TUChar *pWBuff, TInt32 wBuffLength) = 0;
    //virtual TInt32  OnSend(CMessageHeader *pMsg) = 0;
    //virtual TInt32  CodeMsg(CMessageHeader *pMsg,TUChar *pWBuff,TInt32 wBuffLength) = 0;
};

}

 #endif
 
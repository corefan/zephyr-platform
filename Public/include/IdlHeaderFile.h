/*-------------------------------------------------------------
 Copyright (C)| 
 File: IdlHeader.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  使用IdlGenerator生成的代码所需的几个宏
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
 
#ifndef __ZEPHYR_IDL_HEADER_FILE_H__
#define __ZEPHYR_IDL_HEADER_FILE_H__

#include "../Interface/Platform/include/IfTaskMgr.h"
#include "../Interface/Platform/include/IfLoggerMgr.h"
#include "../Interface/Platform/include/IfFinalizer.h"


#define GET_RAWMSG(address) ((CMessageHeader *)( (TUChar *)(address) - (TUInt32)(&((CMessageHeader *)0)->m_msgInfo)))


#define DECALRE_HANDLE_INTERFCE virtual TInt32  OnRecv(CMessageHeader *pMsg);

#define REGISTER_INTERFACE_2_GW(CONNECTION_PLUGIN,IF_NAME) CONNECTION_PLUGIN.RegisterService(GetMyDoid(),IF_NAME##ServiceId,(IF_NAME##ServiceIdBegin),(IF_NAME##ServiceIdEnd),0);

#define REGISTER_INTERFACE_2_GW_WITH_PRIORITY(CONNECTION_PLUGIN,IF_NAME,PRIORITY) CONNECTION_PLUGIN.RegisterService(GetMyDoid(),IF_NAME##ServiceId,(IF_NAME##ServiceIdBegin),(IF_NAME##ServiceIdEnd),PRIORITY);

#define IMPLEMENT_START_HANDLE_INTERFACE(CLASS_NAME) TInt32  CLASS_NAME##::OnRecv(CMessageHeader *pMsg) \
    { \

#define IMPLEMENT_HANDLE_INTERFACE(IF_NAME) if ( IF_NAME##Skeleton::IsMine(pMsg)) \
{ \
    IF_NAME##Skeleton tSkeleton(this); \
    return tSkeleton.HandleMsg(pMsg); \
}

#define  IMPLEMENT_END_HANDLE_INTERFACE(CLASS_NAME)  \
    return CLASS_NAME##::OnRecvUnacceptableMsg(pMsg); \
}



#define GET_REMOTE_STUB_PT(pointer,IfClass,pRemoteDoid) \
    IfClass##Stub tmp_##IfClass##Stub;\
    tmp_##IfClass##Stub.Init((GetSkeleton()),(pRemoteDoid));\
{\
    pointer = &tmp_##IfClass##Stub;\
}\


#define GET_REMOTE_CALLER(pointer,IfClass) \
    IfClass##Stub tmp_##IfClass##Stub;\
    tmp_##IfClass##Stub.Init((GetSkeleton()),(GetCallerDoid()));\
{\
    pointer = &tmp_##IfClass##Stub;\
}\

#define GET_REMOTE_STUB_BY_DOID(pointer,IfClass,remoteDoid) \
    IfClass##Stub tmp_##IfClass##Stub;\
    tmp_##IfClass##Stub.Init(GetSkeleton(),(& remoteDoid));\
{\
    pointer = &tmp_##IfClass##Stub;\
}\


#endif

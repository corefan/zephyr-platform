/*-------------------------------------------------------------
 Copyright (C)| 2008-2010 TDGame Info. Co. Ltd.
 File: SysInc.h
 Author: Yipeng.Zhang
 Date: 2009-3-16 16:09:12
 Description: this is a list file for win32 or linux platform
 Others:
 -------------------------------------------------------------*/

#ifndef __SYS_INCLUDE_H__
#define __SYS_INCLUDE_H__

#include "SysMacros.h"

#ifdef _MSC_VER
// win include
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <winerror.h>

#define I64_FMT "%lld"


#else

#error you must specify WIN32 specification
#endif

// linux include
#else
#include <pthread.h>


#define NOERROR 0
#define E_FAIL  0x80005
#define SOCKET_ERROR -1
typedef struct hostent HOSTENT;
typedef struct in_addr IN_ADDR;


#define I64_FMT "%I64d"



#endif



#endif

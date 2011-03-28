/*-------------------------------------------------------------
 Copyright (C)| 
 File: IfOrb.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description: ORB�ӿ�,���Է���IfStub
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/

#ifndef __ZEPHYR_IF_ORB_H__
#define __ZEPHYR_IF_ORB_H__


#include "IfStub.h"
#include "IfSkeleton.h"

namespace Zephyr
{

class IfOrb
{
public:
    //ע��Obj
    IfSkeleton* RegisterObj(IfObj *pIfObj) = 0;
    //ע�������ObjIdfx
    IfSkeleton* RegisterObj(IfObj *pIfObj,TInt32 nObjIdx) = 0;
    //ע��
    void    UnRegisterObj(IfSkeleton *pSkeleton) = 0;


    //����ΪService׼����,ÿ��comm��ֻ����һ��.Ϊ�������������.
    TInt32 RegisterRun(IfObj *pObj,TUInt32 nGapInMs) = 0;
    //ʱ�����,��ȡһ�ξ����ˣ���������
    virtual const CClock *GetClock() = 0;
//  ����ŵ�Ӧ�ò�ȥ�� 
//     //����Service
//     CDoid   FindService(TCHAR *pName) = 0;
//     CDoid   FindService(TUInt32 nServiceId) = 0;
    
    
};


}

#endif

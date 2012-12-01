#ifndef __ZEPHYR_CLIENT_SKELETON_H__
#define __ZEPHYR_CLIENT_SKELETON_H__

#include "Public/include/TypeDef.h"
#include "Public/Interface/Platform/include/IfSkeleton.h"
#include "Public/Interface/Platform/include/IfNet.h"
#include "Public/Interface/App/include/IfCryptor.h"

namespace Zephyr
{

class CClientSkeleton : public IfSkeleton
{
private:
    CMessageHeader *m_pBuffer;
    TUInt32 m_uMaxLength;
    IfConnection *m_pIfConnection;
    CDoid m_tFakeDoid;
public:
    
    CClientSkeleton();
    ~CClientSkeleton();
    TInt32 InitMem(TUInt32 uMaxLength);

    void Reset(IfConnection *pConnection)
    {
        m_pIfConnection = pConnection;
    }
    
    virtual CDoid *GetMyDoid();
    //获得后自己初始化
    virtual CMessageHeader *PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid* destDoid,TInt32 destDoidNum,bool bRearrangeDest);
    //发送消息
    virtual TInt32  SendMsg(CMessageHeader *pMsg);
};

}

#endif

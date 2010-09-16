#include "AppConnection.h"
#include "stdio.h"
#include "stdlib.h"
using namespace Zephyr;


int CAppConnection::sm_averageMsgLen = 0;
TInt32 CAppConnection::Run()
{
    long long len = m_msgRecved + m_passiveSendNr;
    if (len > m_msgSend)
    {
        len -= m_msgSend;
    }
    if (m_pIfConnection)
    {
        static int buff[32*1024];
        int sendLen = (rand() % sm_averageMsgLen);
        sendLen <<= 1;
        sendLen &= 0xFFFFFFFC;
        
        for (int i=0;i<sendLen;i+=4)
        {
            buff[i] = sendLen+i;
        }
        
        int ret = m_pIfConnection->SendMsg((TUChar *)buff,sendLen);
        if (SUCCESS < ret)
        {
            m_msgSend += sendLen;
        }
        return sendLen;
    }
    return SUCCESS;
}

TInt32 CAppConnection::OnRecv(TUChar *pMsg, TUInt32 msgLen)
{
    TUInt32 *pLen = (TUInt32*)pMsg;
    if (*pLen != msgLen)
    {
        printf("Wrong size!");
        return msgLen;
    }
    TUInt32 begin = *pLen;
    for (int i=0;i<msgLen;i+=sizeof(TUInt32))
    {
        if (*pLen != (begin + i))
        {
            printf("Wrong Size!");
            return msgLen;
        }
    }
    m_msgRecved += msgLen;
    Run();
    return SUCCESS;
}
    //virtual TInt32 OnRecvIn2Piece(TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
    //网络层会自动从factory生成parser和crypter,请应用层对这连个东西进行设置
    //应用层应该明确知道IfParser 和 IfCryptor的实现类，并在OnConnected的时候对其进行设置.
    //如果返回值en_if_connection_callback_on_connected_release_parser 设置了，那么parser会被释放
    //如果返回值en_if_connection_callback_on_connected_release_cryptor 设置了，那么pCryptor会被释放
    //IfConnection *pIfConnection在连接实际建立的时候再传给应用层。
TInt32 CAppConnection::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    m_pIfConnection = pIfConnection;
    return SUCCESS;
}

    //任何socket异常都会自动关闭网络连接
TInt32 CAppConnection::OnDissconneted(TInt32 erroCode)
{
    m_pIfConnection = NULL;
    return SUCCESS;
}


    //
TInt32 CAppConnection::SendMsg(TUChar *pMsg,TUInt32 msgLen)
{
    if (m_pIfConnection)
    {
         return m_pIfConnection->SendMsg(pMsg,msgLen);
    }
    return NOT_INITIALIZED;
   
}

    //获取连接信息
CConPair *CAppConnection::GetConnectionInfo()
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->GetConnectionInfo();
    }
    return NULL;
}
    //设置是否需要Negla算法
TInt32 CAppConnection::NeedNoDelay(const char chOpt)
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->NeedNoDelay(chOpt);
    }
    return NOT_INITIALIZED;
}
    //获取连接状态.
EnConnectionState CAppConnection::GetConnctionState()
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->GetConnctionState();
    }
    return connection_is_not_in_use;
}
    //用以获取还未发送的数据的长度
TInt32 CAppConnection::GetPendingDataLen()
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->GetPendingDataLen();
    }
    return NOT_INITIALIZED;
}

    //调用这个后，就可以将IfConnectionCallBack释放.Net不会继续回调该接口.
TInt32 CAppConnection::Disconnect()
{
    if (m_pIfConnection)
    {
        TInt32 ret = m_pIfConnection->Disconnect();
        m_pIfConnection = NULL;
        return ret;
    }
    return SUCCESS;
}




#include "include\CommConnection.h"
#include "..\Public\include\SysMacros.h"
#include "include\CommLogger.h"

namespace Zephyr
{

CCommConnection::CCommConnection()
{
    m_pIfConnection = NULL;
    m_usedTime      = 0;
    //#ifdef _DEBUG
    m_msgRecved     = 0;
    m_msgSend       = 0;
}

CCommConnection::~CCommConnection()
{
    //啥都不做
}

TBool CCommConnection::IsActived()
{
    if (m_pIfConnection)
    {
        if (connection_is_using == m_pIfConnection->GetConnctionState())
        {
            return SUCCESS;
        }
    }
    return  FALSE;
}

TInt32 CCommConnection::OnInit()
{
    if (m_pIfConnection)
    {
        COMM_EXCPETION_LOG(COMM_CONNECTION_REINITED,"Connection reinited!");
        m_pIfConnection->Disconnect();
    }
    m_pIfConnection = NULL;
    m_usedTime      = 0;
    //#ifdef _DEBUG
    m_msgRecved     = 0;
    m_msgSend       = 0;
    return SUCCESS;
}

TInt32 CCommConnection::OnFinal()
{
    if (m_pIfConnection)
    {
        m_pIfConnection->Disconnect();
    }
    m_pIfConnection = NULL;
    m_usedTime      = 0;
    //#ifdef _DEBUG
    m_msgRecved     = 0;
    m_msgSend       = 0;
    return SUCCESS;
}

TInt32 CCommConnection::Run()
{
    TInt32 runCnt = 0;
    if (m_pIfConnection)
    {
        if (IsActived())
        {
            
        }
    }
    return runCnt;
}

TInt32 CCommConnection::OnRecv(TUChar *pMsg, TUInt32 msgLen)
{
    TInt32 runCnt = 0;
    return runCnt;
}

//virtual TInt32 OnRecvIn2Piece(TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
//网络层会自动从factory生成parser和crypter,请应用层对这连个东西进行设置
//应用层应该明确知道IfParser 和 IfCryptor的实现类，并在OnConnected的时候对其进行设置.
//如果返回值en_if_connection_callback_on_connected_release_parser 设置了，那么parser会被释放
//如果返回值en_if_connection_callback_on_connected_release_cryptor 设置了，那么pCryptor会被释放
//IfConnection *pIfConnection在连接实际建立的时候再传给应用层。
TInt32 CCommConnection::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    return SUCCESS;
}

//任何socket异常都会自动关闭网络连接
TInt32 CCommConnection::OnDissconneted(TInt32 erroCode)
{
    return SUCCESS;
}


//
TInt32 CCommConnection::SendMsg(TUChar *pMsg,TUInt32 msgLen)
{
    return SUCCESS;
}

//获取连接信息
CConPair *CCommConnection::GetConnectionInfo()
{
    return SUCCESS;
}
//设置是否需要Negla算法
TInt32 CCommConnection::NeedNoDelay(const char chOpt)
{
    return SUCCESS;
}
//获取连接状态.
EnConnectionState CCommConnection::GetConnctionState()
{
    return connection_is_not_in_use;
}
//用以获取还未发送的数据的长度
TInt32 CCommConnection::GetPendingDataLen()
{
    return 0;
}

//调用这个后，就可以将IfConnectionCallBack释放.Net不会继续回调该接口.
TInt32 CCommConnection::Disconnect()
{
    return SUCCESS;
}

}

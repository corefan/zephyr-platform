#include "include\CommConnection.h"
#include "..\Public\include\SysMacros.h"
#include "include\CommLogger.h"

#include "include\CommMgr.h"

#include "include\IpMaps.h"

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
    m_nVirtualIp    = 0;
    m_nNodeId       = 0;
    m_pCommMgr      = NULL;
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
    m_pCommMgr      = NULL;
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
    if (m_pIpMapItem)
    {
        m_pIpMapItem->OnUsed(m_pCommMgr->GetTimeSystem()->GetLocalTime());
    }
    return m_pCommMgr->SendMsg(pMsg,msgLen);
}

//virtual TInt32 OnRecvIn2Piece(TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
//网络层会自动从factory生成parser和crypter,请应用层对这连个东西进行设置
//应用层应该明确知道IfParser 和 IfCryptor的实现类，并在OnConnected的时候对其进行设置.
//如果返回值en_if_connection_callback_on_connected_release_parser 设置了，那么parser会被释放
//如果返回值en_if_connection_callback_on_connected_release_cryptor 设置了，那么pCryptor会被释放
//IfConnection *pIfConnection在连接实际建立的时候再传给应用层。
TInt32 CCommConnection::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    //连接好了，那就
    //走到这儿，那nodeId vip 都应该是准备好了的啦~
    if (m_pCommMgr)
    {
        m_pIfConnection = pIfConnection;
        m_pCommMgr->OnConnected(this);
        return SUCCESS;
    }
    else
    {
        //不应该走到这儿
    }
    return FAIL;
}

//任何socket异常都会自动关闭网络连接
TInt32 CCommConnection::OnDissconneted(TInt32 erroCode)
{
    return SUCCESS;
}

//调用这个后，就可以将IfConnectionCallBack释放.Net不会继续回调该接口.
TInt32 CCommConnection::Disconnect()
{
    if (m_pIfConnection)
    {
        m_pIfConnection->Disconnect();
    }
    m_pIfConnection = NULL;
    //内存由mgr负责释放
    return SUCCESS;
}

}

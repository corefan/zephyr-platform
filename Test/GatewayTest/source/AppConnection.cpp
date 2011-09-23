#include "AppConnection.h"
#include "stdio.h"
#include "stdlib.h"
#include <windows.h>
#include "AppConnectionMgr.h"
#include <iostream> 
#include <time.h> 
#include "Public/include/IdlHeaderFile.h"
#include "Public/include/Message.h"
#include "../include/GatewayTestErrorCode.h"
using namespace Zephyr;



CAppConnection::CAppConnection()
{
    m_pIfConnection = NULL;
    m_msgRecved = 0;
    m_msgSend = 0;
}



TInt32 CAppConnection::OnInit()
{
    m_msgRecved = 0;
    m_msgSend = 0;
    m_pIfConnection = NULL;
    m_connectedTime = 0;
    //呃，这儿又是一个bug，因为转的很快，time(NULL)都是相同值，所以下面的rand()都是相同值
    //srand(time(NULL));
    m_nTestStep = 0;
    return SUCCESS;
}
TInt32 CAppConnection::OnFinal()
{
//     m_msgRecved = 0;
//     m_msgSend = 0;
//     m_passiveSendNr = 0;
//     m_pIfConnection = NULL;
    return SUCCESS;
}



TInt32 CAppConnection::OnRecv(TUChar *pMsg, TUInt32 msgLen)
{
    #ifdef __PRINT_DEBUG_INFO__
    printf("[CAppConnection::OnRecv]");
    #endif
    TUInt32 *pLen = (TUInt32*)pMsg;
    if (*pLen != msgLen)
    {
        printf("Wrong size!");
        return msgLen;
    }
    TUInt32 begin = *pLen;
    for (int i=0;i<(msgLen>>2);++i)
    {
        if (*pLen != (begin + i))
        {
            printf("Wrong Size!");
            return msgLen;
        }
        ++pLen;
    }
    m_msgRecved += msgLen;
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
//#ifdef __PRINT_DEBUG_INFO__
    //printf("[CAppConnection::OnConnected]");
//#endif
    m_pIfConnection = pIfConnection;
    m_msgRecved = 0;
    m_msgSend = 0;
    m_nTestStep = 2;//
    return SUCCESS;
}

    //任何socket异常都会自动关闭网络连接
TInt32 CAppConnection::OnDissconneted(TInt32 erroCode)
{
//#ifdef __PRINT_DEBUG_INFO__
    //printf("[CAppConnection::OnDissconneted]");
//#endif
    //if (!m_passiveSendNr)
    {
        //m_pConnectionMgr->ReleaseConnection(this); //不释放，只是存着
    }
    m_pIfConnection = NULL;
    m_nStepStartTime = m_pConnectionMgr->GetClock()->GetLocalTime();
    m_nTestStep = 0; //转为0
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
        OnDissconneted(-1);
        return ret;
    }
    return SUCCESS;
}



TInt32 CAppConnection::HandleCompactMsg(TUChar *pMsg,TInt32 nMsgLength)
{
    CMessageHeader::UnMsgInfo *pInfo = (CMessageHeader::UnMsgInfo *)pMsg;
    //可以一样处理的
    CMessageHeader *pMsgHeader = GET_RAWMSG(pInfo);
    //获取
    //IMPLEMENT_HANDLE_INTERFACE(IfConnecting);
    return SUCCESS;
}

TInt32 CAppConnection::Routine()
{
    switch (m_nTestStep)
    {
    case en_disconnected:
        {
            //判断时间，准备连接
            return Run0();
        }
        break;
    case en_trying_conntecting:
        {
            //等待链接
            return Run1();
        }
        break;
    case en_send_authentication:
        {
            //连接成功后,发送握手
            return Run2();
        }
        break;
    case en_wait_authentication_resp:
        {
            //等待握手相应
            return Run3();
        }
        break;
    case 4:
        {
        }
        break;
    }
    return SUCCESS;
}

TInt32 CAppConnection::Run0()
{
    if (m_pConnectionMgr->GetClock()->GetTimeGap(m_nStepStartTime) > 50000)
    {
        //如果短线超过50秒，则重连
        return RETRY_CONNECTING;
    }
    return SUCCESS;
}

TInt32 CAppConnection::Run1()
{
    if (m_pConnectionMgr->GetClock()->GetTimeGap(m_nStepStartTime) > 800) //800ms之内必须连接成功
    {
        Disconnect();
    }
    return SUCCESS;
}


TInt32 CAppConnection::Run2()
{
    return SUCCESS;
}

TInt32 CAppConnection::Run3()
{
    return SUCCESS;
}

void CAppConnection::OnTryConnecting()
{
    m_nTestStep = 1;
    m_nStepStartTime = m_pConnectionMgr->GetClock()->GetLocalTime();
}
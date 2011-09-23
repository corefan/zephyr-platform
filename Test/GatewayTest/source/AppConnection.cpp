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
    //�����������һ��bug����Ϊת�ĺܿ죬time(NULL)������ֵͬ�����������rand()������ֵͬ
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
    //�������Զ���factory����parser��crypter,��Ӧ�ò��������������������
    //Ӧ�ò�Ӧ����ȷ֪��IfParser �� IfCryptor��ʵ���࣬����OnConnected��ʱ������������.
    //�������ֵen_if_connection_callback_on_connected_release_parser �����ˣ���ôparser�ᱻ�ͷ�
    //�������ֵen_if_connection_callback_on_connected_release_cryptor �����ˣ���ôpCryptor�ᱻ�ͷ�
    //IfConnection *pIfConnection������ʵ�ʽ�����ʱ���ٴ���Ӧ�ò㡣
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

    //�κ�socket�쳣�����Զ��ر���������
TInt32 CAppConnection::OnDissconneted(TInt32 erroCode)
{
//#ifdef __PRINT_DEBUG_INFO__
    //printf("[CAppConnection::OnDissconneted]");
//#endif
    //if (!m_passiveSendNr)
    {
        //m_pConnectionMgr->ReleaseConnection(this); //���ͷţ�ֻ�Ǵ���
    }
    m_pIfConnection = NULL;
    m_nStepStartTime = m_pConnectionMgr->GetClock()->GetLocalTime();
    m_nTestStep = 0; //תΪ0
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

    //��ȡ������Ϣ
CConPair *CAppConnection::GetConnectionInfo()
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->GetConnectionInfo();
    }
    return NULL;
}
    //�����Ƿ���ҪNegla�㷨
TInt32 CAppConnection::NeedNoDelay(const char chOpt)
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->NeedNoDelay(chOpt);
    }
    return NOT_INITIALIZED;
}
    //��ȡ����״̬.
EnConnectionState CAppConnection::GetConnctionState()
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->GetConnctionState();
    }
    return connection_is_not_in_use;
}
    //���Ի�ȡ��δ���͵����ݵĳ���
TInt32 CAppConnection::GetPendingDataLen()
{
    if (m_pIfConnection)
    {
        return m_pIfConnection->GetPendingDataLen();
    }
    return NOT_INITIALIZED;
}

    //��������󣬾Ϳ��Խ�IfConnectionCallBack�ͷ�.Net��������ص��ýӿ�.
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
    //����һ�������
    CMessageHeader *pMsgHeader = GET_RAWMSG(pInfo);
    //��ȡ
    //IMPLEMENT_HANDLE_INTERFACE(IfConnecting);
    return SUCCESS;
}

TInt32 CAppConnection::Routine()
{
    switch (m_nTestStep)
    {
    case en_disconnected:
        {
            //�ж�ʱ�䣬׼������
            return Run0();
        }
        break;
    case en_trying_conntecting:
        {
            //�ȴ�����
            return Run1();
        }
        break;
    case en_send_authentication:
        {
            //���ӳɹ���,��������
            return Run2();
        }
        break;
    case en_wait_authentication_resp:
        {
            //�ȴ�������Ӧ
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
        //������߳���50�룬������
        return RETRY_CONNECTING;
    }
    return SUCCESS;
}

TInt32 CAppConnection::Run1()
{
    if (m_pConnectionMgr->GetClock()->GetTimeGap(m_nStepStartTime) > 800) //800ms֮�ڱ������ӳɹ�
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
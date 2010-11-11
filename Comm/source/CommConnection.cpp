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
    //ɶ������
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
//�������Զ���factory����parser��crypter,��Ӧ�ò��������������������
//Ӧ�ò�Ӧ����ȷ֪��IfParser �� IfCryptor��ʵ���࣬����OnConnected��ʱ������������.
//�������ֵen_if_connection_callback_on_connected_release_parser �����ˣ���ôparser�ᱻ�ͷ�
//�������ֵen_if_connection_callback_on_connected_release_cryptor �����ˣ���ôpCryptor�ᱻ�ͷ�
//IfConnection *pIfConnection������ʵ�ʽ�����ʱ���ٴ���Ӧ�ò㡣
TInt32 CCommConnection::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    return SUCCESS;
}

//�κ�socket�쳣�����Զ��ر���������
TInt32 CCommConnection::OnDissconneted(TInt32 erroCode)
{
    return SUCCESS;
}


//
TInt32 CCommConnection::SendMsg(TUChar *pMsg,TUInt32 msgLen)
{
    return SUCCESS;
}

//��ȡ������Ϣ
CConPair *CCommConnection::GetConnectionInfo()
{
    return SUCCESS;
}
//�����Ƿ���ҪNegla�㷨
TInt32 CCommConnection::NeedNoDelay(const char chOpt)
{
    return SUCCESS;
}
//��ȡ����״̬.
EnConnectionState CCommConnection::GetConnctionState()
{
    return connection_is_not_in_use;
}
//���Ի�ȡ��δ���͵����ݵĳ���
TInt32 CCommConnection::GetPendingDataLen()
{
    return 0;
}

//��������󣬾Ϳ��Խ�IfConnectionCallBack�ͷ�.Net��������ص��ýӿ�.
TInt32 CCommConnection::Disconnect()
{
    return SUCCESS;
}

}

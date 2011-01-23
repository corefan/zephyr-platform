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
//�������Զ���factory����parser��crypter,��Ӧ�ò��������������������
//Ӧ�ò�Ӧ����ȷ֪��IfParser �� IfCryptor��ʵ���࣬����OnConnected��ʱ������������.
//�������ֵen_if_connection_callback_on_connected_release_parser �����ˣ���ôparser�ᱻ�ͷ�
//�������ֵen_if_connection_callback_on_connected_release_cryptor �����ˣ���ôpCryptor�ᱻ�ͷ�
//IfConnection *pIfConnection������ʵ�ʽ�����ʱ���ٴ���Ӧ�ò㡣
TInt32 CCommConnection::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    //���Ӻ��ˣ��Ǿ�
    //�ߵ��������nodeId vip ��Ӧ����׼�����˵���~
    if (m_pCommMgr)
    {
        m_pIfConnection = pIfConnection;
        m_pCommMgr->OnConnected(this);
        return SUCCESS;
    }
    else
    {
        //��Ӧ���ߵ����
    }
    return FAIL;
}

//�κ�socket�쳣�����Զ��ر���������
TInt32 CCommConnection::OnDissconneted(TInt32 erroCode)
{
    return SUCCESS;
}

//��������󣬾Ϳ��Խ�IfConnectionCallBack�ͷ�.Net��������ص��ýӿ�.
TInt32 CCommConnection::Disconnect()
{
    if (m_pIfConnection)
    {
        m_pIfConnection->Disconnect();
    }
    m_pIfConnection = NULL;
    //�ڴ���mgr�����ͷ�
    return SUCCESS;
}

}

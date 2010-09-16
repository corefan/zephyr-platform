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
    //�������Զ���factory����parser��crypter,��Ӧ�ò��������������������
    //Ӧ�ò�Ӧ����ȷ֪��IfParser �� IfCryptor��ʵ���࣬����OnConnected��ʱ������������.
    //�������ֵen_if_connection_callback_on_connected_release_parser �����ˣ���ôparser�ᱻ�ͷ�
    //�������ֵen_if_connection_callback_on_connected_release_cryptor �����ˣ���ôpCryptor�ᱻ�ͷ�
    //IfConnection *pIfConnection������ʵ�ʽ�����ʱ���ٴ���Ӧ�ò㡣
TInt32 CAppConnection::OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor)
{
    m_pIfConnection = pIfConnection;
    return SUCCESS;
}

    //�κ�socket�쳣�����Զ��ر���������
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
        m_pIfConnection = NULL;
        return ret;
    }
    return SUCCESS;
}




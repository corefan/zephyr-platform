/*-------------------------------------------------------------
 Copyright (C)| 
 File: CommConnection.h
 Author: Zephyr Shannon
 Date: 2010-3-2 10:31:50
 Version: 
 Description:  ���������
 Others: 
 Function List: 
 Histroy: 
 -------------------------------------------------------------*/
#ifndef __ZEPHYR_COMM_COMM_CONNECTION_H__
#define __ZEPHYR_COMM_COMM_CONNECTION_H__

#include "IfNetApp.h"
#include "IfNet.h"

namespace Zephyr
{


class CCommConnection : public IfConnectionCallBack
{
private:
    IfConnection*   m_pIfConnection;
    TUInt32         m_usedTime;
    //#ifdef _DEBUG
    TUInt64         m_msgRecved;
    TUInt64         m_msgSend;

    //#endif
     DECLARE_CLASS_LIST (CCommConnection)
public:
    CCommConnection();
    ~CCommConnection();
    TBool IsActived();
    TInt32 OnInit();
    TInt32 OnFinal();

    TInt32 Run();

    virtual TInt32 OnRecv(TUChar *pMsg, TUInt32 msgLen);
    //virtual TInt32 OnRecvIn2Piece(TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2) = 0;
    //�������Զ���factory����parser��crypter,��Ӧ�ò��������������������
    //Ӧ�ò�Ӧ����ȷ֪��IfParser �� IfCryptor��ʵ���࣬����OnConnected��ʱ������������.
    //�������ֵen_if_connection_callback_on_connected_release_parser �����ˣ���ôparser�ᱻ�ͷ�
    //�������ֵen_if_connection_callback_on_connected_release_cryptor �����ˣ���ôpCryptor�ᱻ�ͷ�
    //IfConnection *pIfConnection������ʵ�ʽ�����ʱ���ٴ���Ӧ�ò㡣
    virtual TInt32 OnConnected(IfConnection *pIfConnection,IfParser *pParser,IfCryptor *pCryptor);

    //�κ�socket�쳣�����Զ��ر���������
    virtual TInt32 OnDissconneted(TInt32 erroCode);


    //
    TInt32 SendMsg(TUChar *pMsg,TUInt32 msgLen) ;

    //��ȡ������Ϣ
    CConPair *GetConnectionInfo();
    //�����Ƿ���ҪNegla�㷨
    TInt32 NeedNoDelay(const char chOpt);
    //��ȡ����״̬.
    EnConnectionState GetConnctionState();
    //���Ի�ȡ��δ���͵����ݵĳ���
    TInt32 GetPendingDataLen();

    //��������󣬾Ϳ��Խ�IfConnectionCallBack�ͷ�.Net��������ص��ýӿ�.
    TInt32 Disconnect();    

    TUInt32 GetFreeBuffLength()
    {
        if (m_pIfConnection)
        {
            return m_pIfConnection->GetFreeBuffLength();
        }
        return 0;
    }
};

}

#endif //__ZEPHYR_COMM_COMM_CONNECTION_H__

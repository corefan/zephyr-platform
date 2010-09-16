#ifndef __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_H__
#define __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_H__

#include "IfNetApp.h"
#include "IfNet.h"

using namespace Zephyr;
class CAppConnection : public IfConnectionCallBack
{
private:
    IfConnection*   m_pIfConnection;
    long long       m_msgRecved;
    long long       m_msgSend;
    int             m_passiveSendNr;
    static int      sm_averageMsgLen;
public:
    TInt32 Run();
    static void SetAverageMsgLen(int len)
    {
        sm_averageMsgLen = len;
    }

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
};



#endif

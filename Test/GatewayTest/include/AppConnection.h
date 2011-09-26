#ifndef __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_H__
#define __ZEPHYR_COMM_WIN_NET_TEST_APP_CONNECTION_H__

#include "IfNetApp.h"
#include "IfNet.h"
#include "Public/tpl/include/TplList.h"
using namespace Zephyr;

class CAppConnectionMgr;
class CAppConnection : virtual public IfConnectionCallBack
{
private:
    IfConnection*   m_pIfConnection;
    CAppConnectionMgr   *m_pConnectionMgr;
    long long       m_msgRecved;
    long long       m_msgSend;

    unsigned int    m_connectedTime;

    unsigned int    m_nTestStep;
    unsigned int    m_nStepStartTime;
    CList<CAppConnection> *m_pBelongs2;
    
public:
    enum EnTestStep
    {
        en_disconnected = 0,      //������ʱ�������
        en_trying_conntecting ,   //��������
        en_send_authentication,  //���ӷ��ɹ�����ʼ�����Ȩ
        en_wait_authentication_resp, //�ȴ���Ȩ��Ӧ
        en_test_01,             //����1
    };
    void SetMgr(CAppConnectionMgr *pMgr)
    {
        m_pConnectionMgr = pMgr;
    }
    CAppConnection();
    TInt32 OnInit();
    TInt32 OnFinal();
    
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

public:
    TInt32 HandleCompactMsg(TUChar *pMsg,TInt32 nMsgLength);
    TInt32 Routine();
    TInt32 Run0();
    TInt32 Run1();
    TInt32 Run2();
    TInt32 Run3();
    void OnTryConnecting();
};



#endif
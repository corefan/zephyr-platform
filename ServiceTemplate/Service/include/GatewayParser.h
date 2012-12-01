#ifndef __ZEPHYR_GATEWAY_PARSER_H__
#define __ZEPHYR_GATEWAY_PARSER_H__

#include "Public/Interface/App/include/IfParser.h"
#include "Public/Interface/App/include/IfCryptor.h"
namespace Zephyr
{

//�ڿͻ��˷�����Ϣ��ʱ�����ǿ��԰�m_destDoid��m_srcDoid��������Ϊ����������������������Ĭ�ϵģ�����Ҫ�ٴ�




class CGatewayParser : public IfParser
                     , public IfCryptor
{
private:
    enum EnParserState
    {
        en_cryptor_not_inited,
        en_cryptor_key_is_setted,
    };
    TUInt16 m_tKeys[64];
    TUChar  *m_pDecrypt2;
    TUInt16 m_enInitState;
    volatile TUInt16 m_uLeftSize;
public:
    CGatewayParser()
    {
        OnInit();
    }
    void OnInit();
    void SetKey(TUInt16 *pKeys);
    virtual TInt32  GetMaxHeaderLength();
    virtual TInt32  GetMinHeaderLength();
    //���GetMaxHeaderLength == GetMinHeaderLength����headerLen����GetMinHeaderLength��
    //ע��GetMaxHeaderLength��GetMinHeaderLengthֻ�����ӳ�ʼ����ʱ�����һ�Σ��Ժ󲻻�ı�
    //����û�������������۵����ݳ���GetMinHeaderLength,��ͻص����������ͬʱ��ʵ���յ������ݳ���(<=GetMaxHeaderLength)д��headerLen��.
    //pNetData ֻ����MaxHeaderLength������,Ӧ�ò����ͷ��Ϣ��Ҫ���ظ�net�˴���Ҫ�ϱ������ݳ���.
    //����>0���ʾ�ɹ���������ʾ��Ҫ�������ֽ���.
    //ÿ�η���һ����Ϣ�ĳ��ȣ�������غ󣬻��ж�������ݣ�Net���ε���
    virtual TInt32  OnRecv(TUChar *pNetData,TInt32 dataLen);

    //������������á����Ҫ�޸���ע���߳����⡣
    virtual TInt32  Encrypt(TUChar *pNetData,TUInt32 dataLength);
    //����,���ʱ������net����,���յ���ʱ�����,pNewData�ڴ����ʹ�þ�̬������ջ����,����㲢��ɾ��
    virtual TInt32  Decrypt(TUChar *pNetData,TUInt32 dataLength);
};

}

#endif

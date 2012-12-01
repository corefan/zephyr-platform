#include "../include/GatewayParser.h"
#include "Public/include/TypeDef.h"
#include "Public/include/SysMacros.h"
#include "Public/include/Message.h"
#include "../CryptLib2/rc2.h"
#include <assert.h>
#pragma warning(push)
#pragma warning(disable:4244)

namespace Zephyr
{
TInt32  CGatewayParser::GetMaxHeaderLength()
{
    return sizeof(CMessageHeader::UnMsgInfo);
}

TInt32  CGatewayParser::GetMinHeaderLength()
{
    return sizeof(CMessageHeader::UnMsgInfo);
}
    //���GetMaxHeaderLength == GetMinHeaderLength����headerLen����GetMinHeaderLength��
    //ע��GetMaxHeaderLength��GetMinHeaderLengthֻ�����ӳ�ʼ����ʱ�����һ�Σ��Ժ󲻻�ı�
    //����û�������������۵����ݳ���GetMinHeaderLength,��ͻص����������ͬʱ��ʵ���յ������ݳ���(<=GetMaxHeaderLength)д��headerLen��.
    //pNetData ֻ����MaxHeaderLength������,Ӧ�ò����ͷ��Ϣ��Ҫ���ظ�net�˴���Ҫ�ϱ������ݳ���.
    //����>0���ʾ�ɹ���������ʾ��Ҫ�������ֽ���.
    //ÿ�η���һ����Ϣ�ĳ��ȣ�������غ󣬻��ж�������ݣ�Net���ε���
TInt32  CGatewayParser::OnRecv(TUChar *pNetData,TInt32 nDataLen)
{
    TInt32 uMsgLength = *((TInt32*)pNetData);
    if ((nDataLen-(TInt32)sizeof(CMessageHeader::UnMsgInfo)) >= uMsgLength)
    {
        return uMsgLength + sizeof(CMessageHeader::UnMsgInfo);
    }
    return 0;
}

void CGatewayParser::OnInit() 
{
    memset(m_tKeys,0,sizeof(m_tKeys));
    m_pDecrypt2 = NULL;
    m_uLeftSize = 0;
    m_enInitState = en_cryptor_not_inited;
}

void CGatewayParser::SetKey(TUInt16 *pKeys)
{
    memcpy(&m_tKeys[0],pKeys,sizeof(TUInt16)*32);
    memcpy(&m_tKeys[32],pKeys,sizeof(TUInt16)*32);
    m_enInitState = en_cryptor_key_is_setted;
    m_uLeftSize = 0;
}


//������������á����Ҫ�޸���ע���߳����⡣
TInt32 CGatewayParser::Encrypt(TUChar *pNetData,TUInt32 dataLength)
{
    return dataLength;
}
//����,���ʱ������net����,���յ���ʱ�����,pNewData�ڴ����ʹ�þ�̬������ջ����,����㲢��ɾ��
TInt32 CGatewayParser::Decrypt(TUChar *pNetData,TUInt32 dataLength)
{
    if (en_cryptor_key_is_setted == m_enInitState)
    {
        TUChar *pDecrytStart(NULL); 
        if (m_uLeftSize)
        {
            if (m_pDecrypt2 == (pNetData - m_uLeftSize))
            {
                dataLength += m_uLeftSize; //����ǻ�ʣ�ĳ���
            }
            else
            {
                assert(0);
            }
            pDecrytStart = m_pDecrypt2;
        }
        else
        {
            pDecrytStart = pNetData;
        }
        m_uLeftSize = (dataLength & 0x00000007);
        TUInt32 u2Decrypt = (dataLength & 0xFFFFFFFF8);
        if (u2Decrypt > 0)
        {
            TUInt32 uDecrypTime = (u2Decrypt >> 3);
            TUChar szPlain[8];
            for (TUInt32 i=0;i<uDecrypTime;++i)
            {
                rc2_decrypt(m_tKeys,szPlain,pDecrytStart);
                memcpy(pDecrytStart,szPlain,8);
                pDecrytStart += 8;
            }
            //m_uLeftSize = dataLength - u2Decrypt;
            m_pDecrypt2 = pDecrytStart;
        }
        else
        {
            m_pDecrypt2 = pDecrytStart;
        }
    }
    return dataLength;
}


}
#pragma warning(pop)
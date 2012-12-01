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
    //如果GetMaxHeaderLength == GetMinHeaderLength，则headerLen就是GetMinHeaderLength，
    //注意GetMaxHeaderLength、GetMinHeaderLength只在连接初始化的时候调用一次，以后不会改变
    //否则得话，如果网络层积累的数据超过GetMinHeaderLength,则就回调这个函数，同时将实际收到的数据长度(<=GetMaxHeaderLength)写在headerLen中.
    //pNetData 只传给MaxHeaderLength的数据,应用层根据头信息，要返回给net此次需要上报的数据长度.
    //返回>0则表示成功，负数表示需要丢弃的字节数.
    //每次返回一条消息的长度，如果返回后，还有多余的数据，Net会多次调用
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


//由网络层来调用。如果要修改请注意线程问题。
TInt32 CGatewayParser::Encrypt(TUChar *pNetData,TUInt32 dataLength)
{
    return dataLength;
}
//解密,这个时间是由net来的,在收到的时候解密,pNewData内存块请使用静态变量或栈变量,网络层并不删除
TInt32 CGatewayParser::Decrypt(TUChar *pNetData,TUInt32 dataLength)
{
    if (en_cryptor_key_is_setted == m_enInitState)
    {
        TUChar *pDecrytStart(NULL); 
        if (m_uLeftSize)
        {
            if (m_pDecrypt2 == (pNetData - m_uLeftSize))
            {
                dataLength += m_uLeftSize; //这个是还剩的长度
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
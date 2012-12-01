#ifndef __ZEPHYR_GATEWAY_PARSER_FACTORY_H__
#define __ZEPHYR_GATEWAY_PARSER_FACTORY_H__

#include"Public/Interface/App/include/IfParserFactory.h"
#include "Public/include/TypeDef.h"
#include "./GatewayParser.h"
#include "Public/Interface/App/include/IfCryptorFactory.h"

namespace Zephyr
{
class CGatewayParserFactory : public IfParserFactory
                            , public IfCryptorFactory
{
private:
    TInt32          m_nParserNr;
    CGatewayParser *m_pCommonParser;
public:
    CGatewayParserFactory();
    ~CGatewayParserFactory();
    TInt32 InitFarctory(TInt32 nNr);
    //����CConPair������һ����������ע�⣬�����������Ӧ�ò���߳������õģ�����IfNet::Run�б��ص�
    virtual IfCryptor *GetCryptor(CConPair *pPair,TUInt32 connectionIdx);
    //�ͷ�IfCryptor
    virtual void ReleaseCryptor(IfCryptor *pIfCryptor);
    //����CConPair������һ���������ע�⣬�����������Ӧ�ò���߳������õģ�����IfNet::Run�б��ص�;
    virtual IfParser *GetParser(CConPair *pPair,TUInt32 connectionIdx);
    virtual void ReleaseParser(IfParser *pParser);
};

}


#endif

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
    //根据CConPair来创建一个加密器，注意，这个方法是由应用层的线程来调用的，即在IfNet::Run中被回调
    virtual IfCryptor *GetCryptor(CConPair *pPair,TUInt32 connectionIdx);
    //释放IfCryptor
    virtual void ReleaseCryptor(IfCryptor *pIfCryptor);
    //根据CConPair来创建一个解包器，注意，这个方法是由应用层的线程来调用的，即在IfNet::Run中被回调;
    virtual IfParser *GetParser(CConPair *pPair,TUInt32 connectionIdx);
    virtual void ReleaseParser(IfParser *pParser);
};

}


#endif

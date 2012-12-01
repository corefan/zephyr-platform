#include "../include/GatewayParserFactory.h"

namespace Zephyr
{

CGatewayParserFactory::CGatewayParserFactory()
{
    m_nParserNr = 0;
    m_pCommonParser = NULL;
}

CGatewayParserFactory::~CGatewayParserFactory()
{
    if (m_pCommonParser)
    {
        delete [] m_pCommonParser;
        m_pCommonParser = NULL;
    }
    m_nParserNr = 0;
}
TInt32 CGatewayParserFactory::InitFarctory(TInt32 nNr)
{
    NEW(m_pCommonParser,CGatewayParser,nNr);
    if (m_pCommonParser)
    {
        m_nParserNr = nNr;
        return SUCCESS;
    }
    return OUT_OF_MEM;
}

//根据CConPair来创建一个加密器，注意，这个方法是由应用层的线程来调用的，即在IfNet::Run中被回调
IfCryptor *CGatewayParserFactory::GetCryptor(CConPair *pPair,TUInt32 connectionIdx)
{
    if (connectionIdx < m_nParserNr)
    {
        return &m_pCommonParser[connectionIdx];
    }
    return NULL;
}
//释放IfCryptor
void CGatewayParserFactory::ReleaseCryptor(IfCryptor *pIfCryptor)
{
    //doing nothing
}

IfParser *CGatewayParserFactory::GetParser(CConPair *pPair,TUInt32 connectionIdx)
{
    if (connectionIdx < m_nParserNr)
    {
        return &m_pCommonParser[connectionIdx];
    }
    return NULL;
}



void CGatewayParserFactory::ReleaseParser(IfParser *pParser)
{
    //doing nothing
}

}
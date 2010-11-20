#include "..\Comm\include\CommLogger.h"
#include "..\Public\include\SysMacros.h"
namespace Zephyr
{

//IfLogger *g_pCommLogger = NULL;

void RecordOneMsg(CMessageHeader *pMsg)
{
    CDoid *pDoid = pMsg->GetSrcDoid();
    CDoid *pDest = pMsg->GetDestDoidByIdx();
    COMM_EXCPETION_LOG(COMM_DROP_MSG,"Drop message from Doid:%u-%i-%u-%u to %u-%u-%u-%u, method_id = %u",
        pDoid->m_nodeId,pDoid->m_virtualIp,pDoid->m_srvId,pDoid->m_objId,
        pDest->m_nodeId,pDest->m_virtualIp,pDest->m_srvId,pDest->m_objId,pMsg->GetMethodId());
    int bodyLen = pMsg->GetBodyLength();
    TUChar *pMsgBody = pMsg->GetBody();
    int recordTo = 0;
    //¼ÇÂ¼body.
    while (bodyLen > 0)
    {
        char buff[2048+4];
        int to;
        if (1024 > (bodyLen - recordTo))
        {
            to = 1024;
        }
        else
        {
            to = (bodyLen - recordTo);
        }
        for (int i=0;i<to;++i)
        {
            char octOne = (pMsgBody[i+recordTo]/16) + '0';
            char octTwo = (pMsgBody[i+recordTo]%16) + '0';
            buff[(i+recordTo)<<1]     = octOne;
            buff[((i+recordTo)<<1)+1] = octTwo;
        }
        buff[((recordTo + to)<<1)] = '\0';
        COMM_EXCPETION_LOG(COMM_DROP_MSG_BODY,"Body[%u]: %s",(recordTo/1024),buff);
        bodyLen -= to;
    }
    int broadcastDoidNr = pMsg->GetBroadcastDoidNr();
    if (broadcastDoidNr)
    {
        pDoid = pMsg->GetBroadcastDoids();
        for (int i=0;i<broadcastDoidNr;++i)
        {
            pDest = pDoid + i;
            COMM_EXCPETION_LOG(COMM_DROP_MSG,"Broadcast doid[%u] is %u-%u-%u-%u",
                i,pDest->m_nodeId,pDest->m_virtualIp,pDest->m_srvId,pDest->m_objId);
        }
    }
    
}

}

#include "Message.h"
#include "sysMacros.h"
#include <string.h>
#include "Tpl/include/tplmap.h"
#include <algo.h>


namespace Zephyr
{
struct TDoidSort
{
    CDoid m_key;
};

TInt32 CMessageHeader::Init(TUInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* pDestDoids,TUInt32 destDoidNum)
{
    if ((destDoidNum > 63) || (NULL == pDestDoids) || (bodyLength < 0) || (methodId > 0x3FFFF))
    {
        return INPUT_PARA_ERROR;
    }
    m_msgInfo = 0;
    //TUInt32 length = sizeof(CMessageHeader) + sizeof(CDoid) * (destDoidNum -1);
    m_msgInfo.m_msgBodyLength = bodyLength;
    m_msgInfo.m_methodId  = methodId;
    m_srcDoid   = srcId;k
    
    if (destDoidNum > 1)
    {	
        sort(pDestDoids,pDestDoids+destDoidNum);
        memcpy((void*)GetBroadcastDoids(),(void*)(pDestDoids+1),(sizeof(CDoid)*(destDoidNum-1)));
    }
    m_destDoid  = *pDestDoids;
    /* do it when actived this message!
    for(int i =0;i<(sizeof(SCTDMessageHeader)-2);i+=2)
    {
        m_checkSum[0] = m_checkSum[0] ^ pBuffer[i];
        m_checkSum[1] = m_checkSum[1] ^ pBuffer[i+1];
    }
    */
    return SUCCESS;
}


TInt32 CMessageHeader::SetBodyLength(TUInt32 bodyLength)
{
	TUInt32 destDoidNum = GetDestDoidNum();
	TUInt32 OldBodyLength = GetBodyLength();
	
	if(bodyLength>OldBodyLength)
	{
	    return FAIL;
	}
	else
	{
		CDoid  *pOldDest = GetMultiDestDoids();
		m_msgInfo.m_msgLength = sizeof(CMessageHeader) + sizeof(CDoid) * (destDoidNum -1) + bodyLength;
		
		CDoid  *pNewDest = GetDestDoidByIdx(1);
		if (destDoidNum > 1)
		{
			memmove(pNewDest,pOldDest,(sizeof(CDoid)*(destDoidNum-1)));
		}
		return SUCCESS;
	}
}
//不检查有效性，有调用者负责
void CMessageHeader::ReInitMsg4Send(TInt32 fromDest,TInt32 to)
{
    if (0 != fromDest)
    {
        memcpy(&m_destDoid,GetDestDoidByIdx(fromDest),sizeof(CMessageHeader));
    }
    else
    {
        return ;
    }
    ++fromDest;
    if(to > fromDest)
    {
        memmove(((TUChar*)this + sizeof(CMessageHeader) + GetBodyLength()),GetDestDoidByIdx(fromDest),sizeof(CMessageHeader)*(to-fromDest));
    }
}

}
//to make sure you could only create the message by msgPool.

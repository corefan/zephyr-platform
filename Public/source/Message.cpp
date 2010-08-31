#include "Message.h"
#include "sysMacros.h"
#include <string.h>

namespace Zephyr
{

TInt32 CMessageHeader::Init(TUInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* pDestDoids,TUInt32 destDoidNum)
{
    if ((destDoidNum > 255) || (NULL == pDestDoids) || (bodyLength < 0) || (methodId > 0x3FFFFFFF))
    {
        return INPUT_PARA_ERROR;
    }
    m_msgInfo = 0;
    TUInt32 length = sizeof(CMessageHeader) + sizeof(CDoid) * (destDoidNum -1);
    m_msgInfo = length + bodyLength;
	m_msgInfo = m_msgInfo | (destDoidNum<<22);
    m_methodId = methodId;
    m_srcDoid = srcId;
    for (TUInt32 i=0;i<destDoidNum;i++)
    {
        CDoid *pDoid = GetDestDoidByIdx(i);
        *pDoid = *(pDestDoids + i);
    }
    m_checkSum[0] = 'S';
    m_checkSum[1] = 'Z';

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
		CDoid  *pOldDest = GetDestDoidByIdx(1);
		m_msgInfo = sizeof(CMessageHeader) + sizeof(CDoid) * (destDoidNum -1) + bodyLength;
		m_msgInfo = m_msgInfo | (destDoidNum<<22);

		CDoid  *pNewDest = GetDestDoidByIdx(1);
		if (destDoidNum > 1)
		{
			memmove(pNewDest,pOldDest,(sizeof(CDoid)*(destDoidNum-1)));
		}
		return SUCCESS;
	}

}

}
//to make sure you could only create the message by msgPool.
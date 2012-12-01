using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
public class IfAuthRespStub : IfAuthResp
{
    int IfAuthResp.RespAuthenticate(int _nResult,ulong _uId)
    {
        int nLength =TypeMarshaller.GetLength( _nResult)+TypeMarshaller.GetLength( _uId);
        CMessage pMsg = new CMessage((uint)IfAuthRespMethodId.IfAuthRespRespAuthenticateMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _nResult);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        if (nUsed != nBufferLen)
        {
            Debug.Log("the nUsed is:"+nUsed);
            TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, sizeof(uint), nUsed);
        }
        CMessage.sm_pSocket.SendMsg(pMsg);
        return nUsed;
    }
    int IfAuthResp.ConfirmDisconneted(CDoid _tMyDoid)
    {
        int nLength =CDoid.GetLength(_tMyDoid);
        CMessage pMsg = new CMessage((uint)IfAuthRespMethodId.IfAuthRespConfirmDisconnetedMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = CDoid.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _tMyDoid);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        if (nUsed != nBufferLen)
        {
            Debug.Log("the nUsed is:"+nUsed);
            TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, sizeof(uint), nUsed);
        }
        CMessage.sm_pSocket.SendMsg(pMsg);
        return nUsed;
    }
}

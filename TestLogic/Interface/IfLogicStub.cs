using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
public class IfLogicStub : IfLogic
{
    int IfLogic.Test(ulong _uParm1,int _n2)
    {
        int nLength =TypeMarshaller.GetLength( _uParm1)+TypeMarshaller.GetLength( _n2);
        CMessage pMsg = new CMessage((uint)IfLogicMethodId.IfLogicTestMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uParm1);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _n2);
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

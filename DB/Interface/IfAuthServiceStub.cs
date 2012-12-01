using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
public class IfAuthServiceStub : IfAuthService
{
    int IfAuthService.Authenticate(uint _uDyncNr,char _pszName,char _pszPwd)
    {
        int nLength =TypeMarshaller.GetLength( _uDyncNr)+TypeMarshaller.GetLength( _pszName)+TypeMarshaller.GetLength( _pszPwd);
        CMessage pMsg = new CMessage((uint)IfAuthServiceMethodId.IfAuthServiceAuthenticateMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uDyncNr);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pszName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pszPwd);
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
    int IfAuthService.ChangePwd(char _pszName,char _pszOldPwd,char _pNewPwd)
    {
        int nLength =TypeMarshaller.GetLength( _pszName)+TypeMarshaller.GetLength( _pszOldPwd)+TypeMarshaller.GetLength( _pNewPwd);
        CMessage pMsg = new CMessage((uint)IfAuthServiceMethodId.IfAuthServiceChangePwdMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pszName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pszOldPwd);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pNewPwd);
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
    int IfAuthService.OnDisconneted(CDoid _tMyDoid)
    {
        int nLength =CDoid.GetLength(_tMyDoid);
        CMessage pMsg = new CMessage((uint)IfAuthServiceMethodId.IfAuthServiceOnDisconnetedMethodId, nLength);
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

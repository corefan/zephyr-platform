using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
public class IfUserSessionStub : IfUserSession
{
    int IfUserSession.ChangePwd(char _pOldPwd,char _pszNewPwd)
    {
        int nLength =TypeMarshaller.GetLength( _pOldPwd)+TypeMarshaller.GetLength( _pszNewPwd);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionChangePwdMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pOldPwd);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _pszNewPwd);
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
    int IfUserSession.ReqGetMyInfo(uint _uLastUpdateTime)
    {
        int nLength =TypeMarshaller.GetLength( _uLastUpdateTime);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetMyInfoMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uLastUpdateTime);
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
    int IfUserSession.ReqGetUserInfo(uint _uUserId)
    {
        int nLength =TypeMarshaller.GetLength( _uUserId);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetUserInfoMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uUserId);
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
    int IfUserSession.ReqGetDepartmentInfo(uint _uDepartmentId)
    {
        int nLength =TypeMarshaller.GetLength( _uDepartmentId);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetDepartmentInfoMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uDepartmentId);
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
    int IfUserSession.ReqGetMenuInfoList(uint _uFatherId)
    {
        int nLength =TypeMarshaller.GetLength( _uFatherId);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetMenuInfoListMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uFatherId);
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
    int IfUserSession.ReqUpdateMenuInfo(TJobInfos _rJobInfos)
    {
        int nLength =TJobInfos.GetLength(_rJobInfos);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqUpdateMenuInfoMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TJobInfos.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _rJobInfos);
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
    int IfUserSession.ReqGetActiveWorkInfo(int _nJobId,uint _uRcdId)
    {
        int nLength =TypeMarshaller.GetLength( _nJobId)+TypeMarshaller.GetLength( _uRcdId);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetActiveWorkInfoMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _nJobId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uRcdId);
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
    int IfUserSession.ReqGetMyWorkList(uint _uFromTime,uint _uToTime)
    {
        int nLength =TypeMarshaller.GetLength( _uFromTime)+TypeMarshaller.GetLength( _uToTime);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetMyWorkListMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uFromTime);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uToTime);
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
    int IfUserSession.ReqGetMenuInfoById(uint _uMenuId)
    {
        int nLength =TypeMarshaller.GetLength( _uMenuId);
        CMessage pMsg = new CMessage((uint)IfUserSessionMethodId.IfUserSessionReqGetMenuInfoByIdMethodId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, 0, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed,pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, _uMenuId);
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

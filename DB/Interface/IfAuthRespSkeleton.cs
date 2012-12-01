using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
public class IfAuthRespSkeleton : CSkeleton
{

    IfAuthResp m_pImplementObj;
    public IfAuthRespSkeleton(IfAuthResp pIf)
    {
        m_pImplementObj = pIf;
    }
    public override int HandleMsg(CMessage pMsg)
    {
        pMsg.UnmarshallHeader();
        switch (pMsg.m_uMsgId)
        {
            case (uint)IfAuthRespMethodId.IfAuthRespRespAuthenticateMethodId:
                {
                    return RespAuthenticate_TInt32_TUInt64(pMsg);
                }
                break;
            case (uint)IfAuthRespMethodId.IfAuthRespConfirmDisconnetedMethodId:
                {
                    return ConfirmDisconneted_CDoid(pMsg);
                }
                break;
        }
        return MacrosAndDef.MSG_NOT_HANDLED;
    }
    int RespAuthenticate_TInt32_TUInt64(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        int _nResult;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _nResult);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        ulong _uId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespAuthenticate(_nResult,_uId);
    }
    int ConfirmDisconneted_CDoid(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        CDoid _tMyDoid;
        nLen = CDoid.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _tMyDoid);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.ConfirmDisconneted(_tMyDoid);
    }
}

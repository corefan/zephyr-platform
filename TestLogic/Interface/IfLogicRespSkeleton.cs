using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
class IfLogicRespSkeleton : CSkeleton
{

    IfLogicResp m_pImplementObj;
    public IfLogicRespSkeleton(IfLogicResp pIf)
    {
        m_pImplementObj = pIf;
    }
    public override int HandleMsg(CMessage pMsg)
    {
        pMsg.UnmarshallHeader();
        switch (pMsg.m_uMsgId)
        {
            case (uint)IfLogicRespMethodId.IfLogicRespRespTestMethodId:
                {
                    return RespTest_TInt32(pMsg);
                }
                break;
        }
        return MacrosAndDef.MSG_NOT_HANDLED;
    }
    int RespTest_TInt32(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        int _nRslt;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _nRslt);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespTest(_nRslt);
    }
}

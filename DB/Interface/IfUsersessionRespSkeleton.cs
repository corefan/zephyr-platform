using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
public class IfUsersessionRespSkeleton : CSkeleton
{

    IfUsersessionResp m_pImplementObj;
    public IfUsersessionRespSkeleton(IfUsersessionResp pIf)
    {
        m_pImplementObj = pIf;
    }
    public override int HandleMsg(CMessage pMsg)
    {
        pMsg.UnmarshallHeader();
        switch (pMsg.m_uMsgId)
        {
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespOnPwdChangedMethodId:
                {
                    return OnPwdChanged_TInt32(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespRespGetMyInfoMethodId:
                {
                    return RespGetMyInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespRespGetUserInfoMethodId:
                {
                    return RespGetUserInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespRespGetDepartInfoMethodId:
                {
                    return RespGetDepartInfo_TUInt32_TChar_pt_TUInt32(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespSendJobListMethodId:
                {
                    return SendJobList_list_tpl_begin_TJobInfos_tpl_end__ref_list_tpl_begin_TJobInfos_tpl_end__ref(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespSendActiveWorkMethodId:
                {
                    return SendActiveWork_TJobInfos_TUInt32_const_TChar_pt(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespRespMenuListMethodId:
                {
                    return RespMenuList_list_tpl_begin_TJobInfos_tpl_end_(pMsg);
                }
                break;
            case (uint)IfUsersessionRespMethodId.IfUsersessionRespRespGetMenuInfoMethodId:
                {
                    return RespGetMenuInfo_TJobInfos_ref(pMsg);
                }
                break;
        }
        return MacrosAndDef.MSG_NOT_HANDLED;
    }
    int OnPwdChanged_TInt32(CMessage pMsg)
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
        return m_pImplementObj.OnPwdChanged(_nResult);
    }
    int RespGetMyInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        uint _uUpdateTime;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uUpdateTime);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        char _pszName;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _pszName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        char _pszApartName;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _pszApartName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        uint _uMyId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uMyId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        uint _uApartId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uApartId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespGetMyInfo(_uUpdateTime,_pszName,_pszApartName,_uMyId,_uApartId);
    }
    int RespGetUserInfo_TUInt32_TChar_pt_TChar_pt_TUInt32_TUInt32(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        uint _uUpdateTime;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uUpdateTime);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        char _pszName;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _pszName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        char _pszApartName;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _pszApartName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        uint _uMyId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uMyId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        uint _uApartId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uApartId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespGetUserInfo(_uUpdateTime,_pszName,_pszApartName,_uMyId,_uApartId);
    }
    int RespGetDepartInfo_TUInt32_TChar_pt_TUInt32(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        uint _uUpdateTime;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uUpdateTime);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        char _pszApartName;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _pszApartName);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        uint _uApartId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uApartId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespGetDepartInfo(_uUpdateTime,_pszApartName,_uApartId);
    }
    int SendJobList_list_tpl_begin_TJobInfos_tpl_end__ref_list_tpl_begin_TJobInfos_tpl_end__ref(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        list<TJobInfos> _tMenu2;
        nLen = list<TJobInfos>.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _tMenu2);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        list<TJobInfos> _tMeun3;
        nLen = list<TJobInfos>.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _tMeun3);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.SendJobList(_tMenu2,_tMeun3);
    }
    int SendActiveWork_TJobInfos_TUInt32_const_TChar_pt(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        TJobInfos _tJobs;
        nLen = TJobInfos.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _tJobs);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        uint _uRcdId;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _uRcdId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        char _pszTitle;
        nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _pszTitle);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.SendActiveWork(_tJobs,_uRcdId,_pszTitle);
    }
    int RespMenuList_list_tpl_begin_TJobInfos_tpl_end_(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        list<TJobInfos> _tList;
        nLen = list<TJobInfos>.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _tList);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespMenuList(_tList);
    }
    int RespGetMenuInfo_TJobInfos_ref(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nLen = 0;
        TJobInfos _tJobInfo;
        nLen = TJobInfos.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _tJobInfo);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.RespGetMenuInfo(_tJobInfo);
    }
}

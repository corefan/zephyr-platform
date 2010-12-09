#include "Communicator.h"
#include "CommLogger.h"
#include "../System/include/SysInc.h"
namespace Zephyr
{

CCommunicator::CCommunicator()
{
    m_pBuff     = NULL;
    m_buffSize  = 0;
    m_pTimeSys = NULL;
}

CCommunicator::~CCommunicator()
{
    DELETEP(m_pBuff);
}

TInt32 CCommunicator::Init(CTimeSystem *pTimeSystem,TUInt32 inPipeSize,TUInt32 outPipeSize,TUInt32 maxMessageSize)
{
    m_pTimeSys = pTimeSystem;
    TInt32 ret = m_inPipe.Init(inPipeSize);
    if (SUCCESS > ret)
    {
        return ret;
    }
    ret = m_outPipe.Init(outPipeSize);
    if (SUCCESS > ret)
    {
        return ret;
    }
    NEW(m_pBuff,TUChar,maxMessageSize);
    if (m_pBuff)
    {
        return SUCCESS;
    }
    return OUT_OF_MEM;
}

TInt32 CCommunicator::InitEventPool(TUInt32 maxEventNr)
{
    return m_eventPool.Init(maxEventNr * sizeof(CConnectionEvent));
}
    //add a new idx , that indicate the pipe to use!
CMessageHeader *CCommunicator::GetMsg(TUInt32 needRetMsgBuff)
{
//     TInt32 len = m_inPipe.GetDataLen();
//     if (len < sizeof(CMessageHeader))
//     {
//         return NULL;
//     }
    TUChar buff[sizeof(CMessageHeader)];
    CMessageHeader *pHeader = (CMessageHeader *)m_inPipe.Peek(buff,sizeof(buff));
    if (pHeader)
    {
        TUInt32 len = m_inPipe.GetDataLen();
        if (len > pHeader->GetLength())
        {
            len  = pHeader->GetLength();
            TUChar *pData = m_inPipe.GetData(len);
            //内存是否是一块？
            if (len > pHeader->GetLength())
            {
                //pHeader = (CMessageHeader *)pData;
            }
            else
            {
                len = pHeader->GetLength();
                TInt32 ret = m_inPipe.ReadData(m_pBuff,len);
                if (SUCCESS > ret)
                {
                    return NULL;
                }
                pHeader = (CMessageHeader *)m_pBuff;
            }
            return pHeader;
        }
    }
    return NULL;
}

void CCommunicator::ReturnMsgBuff(CMessageHeader *pMsg)
{
    if (m_pBuff != (TUChar*)pMsg)
    {
        m_inPipe.ReturnMsgBuff((TUChar*)pMsg,pMsg->GetLength());
    }
}

CMessageHeader *CCommunicator::PrepareMsg(TInt32 bodyLength,TUInt32 methodId,CDoid srcId,CDoid* pDestDoid,TInt32 destDoidNum)
{
    TUInt32 len = sizeof(CMessageHeader) + (destDoidNum-1)*sizeof(CDoid);
    TUInt32 getLen = len;
    CMessageHeader *pRtn = (CMessageHeader *)m_outPipe.PrepareMsg(getLen);
    if (getLen < len)
    {
        pRtn = (CMessageHeader *)m_pBuff;
    }
    len = pRtn->Init(bodyLength,methodId,srcId,pDestDoid,destDoidNum);
    if (SUCCESS > len)
    {
        return NULL;
    }
    return pRtn;
}

    //virtual int SendMsg(SCTDMessageHeader *,bool bNeedCopy = false) = 0;
    //do not need the para bNeedCopy any more, I will check it!`
TInt32 CCommunicator::SendMsg(CMessageHeader *pMsg)
{
    if (m_pBuff != (TUChar*)pMsg)
    {
        return m_outPipe.ConfirmAddMsg((TUChar*)pMsg,pMsg->GetLength());
    }
    return m_outPipe.WriteData((TUChar*)pMsg,pMsg->GetLength());
}

    //application should not call this !!! called by work thread only! or else some events would lost!
CConnectionEvent CCommunicator::GetConnectionEvent(TInt32& result)
{
    CConnectionEvent tmp;
    result = m_eventPool.ReadData((TUChar*)&tmp,sizeof(tmp));
    return tmp;
}

TUInt32 CCommunicator::GetTime()
{
    return m_pTimeSys->GetTimeNow();
}

TInt32 CCommunicator::AddNetMsg(CMessageHeader *pMsg)
{
    TInt32 freeLen = m_inPipe.GetFreeLen();
    if (freeLen > pMsg->GetLength())
    {
        return m_inPipe.WriteData((TUChar*)pMsg,pMsg->GetLength());
    }
    else
    {
        TUInt32 timeNow = m_pTimeSys->GetTimeNow();
        if (timeNow >= m_nLastBlockTimes)
        {
            TUInt32 gap = timeNow - m_nLastBlockTimes;
            if (gap > 1000)
            {
                m_nLastBlockTimes = timeNow;
                Sleep(15);
                //再试一次
                freeLen = m_inPipe.GetFreeLen();
                if (freeLen > pMsg->GetLength())
                {
                    return m_inPipe.WriteData((TUChar*)pMsg,pMsg->GetLength());
                }
                //else
                //{
                      //扔掉消息
                //}
                
            }
            //else
            //{
                //扔掉消息
            //}
        }
    }
    //记下日志，然后扔掉
    RecordOneMsg(pMsg);
    return pMsg->GetLength();
}

}

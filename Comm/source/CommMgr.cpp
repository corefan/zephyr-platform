#include "CommMgr.h"
#include "SettingFile.h"
#include "..\Public\include\NetCenter.h"
#include "..\System\include\SysInc.h"
namespace Zephyr
{


TInt32 CCommMgr::Init(IfTaskMgr *pTaskMgr,const TChar *pConfigName/* =szDefaultLoggerName */)
{
    //初始化网络底层
    
    //先读取配置
    TInt32 ret = m_ipMaps.Init(pConfigName);
    if (SUCCESS > ret)
    {
        return ret;
    }
    try
    {
        m_pParserFactory = new CMsgParserFactory(m_ipMaps.m_nrOfVirtualIp + 5);
    }
    catch (...)
    {
    }
    if (!m_pParserFactory)
    {
#ifdef _DEBUG
        printf("Can not allocate mem for m_pParserFactory!");
#endif
        return OUT_OF_MEM;
    }
    m_pNet = CreateNet(pTaskMgr,this,m_pParserFactory,NULL,(m_ipMaps.m_nrOfVirtualIp + 5)/*多加5个*/);
    if (!m_pNet)
    {
#ifdef _DEBUG
        printf("Create m_pNet Failed!");
#endif
        return OUT_OF_MEM;
    }
    NEW(m_ppConnections,CCommConnection*,m_ipMaps.m_nrOfVirtualIp + 5);
    if (!m_ppConnections)
    {
#ifdef _DEBUG
        printf("Can not allocate memory for m_ppConnections");
#endif
        return OUT_OF_MEM;
    }
	//主动连接vip比自己小的所有机器，每台机器重启后都是这个顺序.并且只尝试重连比自己vip小的机器
    if (m_ipMaps.m_nrOfNodes > 1)
    {
        for (int i = 0;i<m_ipMaps.m_nrOfNodes;++i)
        {
            CCommConnection *pConnection = m_connectionPool.GetItem();
            if (!pConnection)
            {
#ifdef _DEBUG
                printf("Can not get Comm Connection");
#endif
                return OUT_OF_MEM;
            }
            TInt32 rtn = m_pNet->Connect(m_ipMaps.m_pVirtualIps[i].m_realIp,m_ipMaps.m_pVirtualIps[m_ipMaps.m_localVirtualIp].m_realIp,
                            m_ipMaps.m_pVirtualIps[i].m_listenPort,m_ipMaps.m_pVirtualIps[m_ipMaps.m_localVirtualIp].m_bindPort,pConnection);
            if (rtn < SUCCESS)
            {
#ifdef _DEBUG
                printf("Connection Failed!");
#endif
                m_connectionPool.ReleaseItem(pConnection);
                return rtn;
            }
            m_ppConnections[i] = pConnection;
            //解决同时连接过多的问题
            if (0 == (i % 32))
            {
                printf("Connecting...");
#ifdef _WIN32
                Sleep(50);
#else  
                usleep(15000);
#endif
                m_pNet->Run(128);
            }
        }
    }
    for (int i=0;i<m_ipMaps.m_localNodeId;++i)
    {
        if (m_ipMaps.m_connectedNode >= 0)
        {
            CCommConnection *p = m_connectionPool.GetItem();
            if (!p)
            {
                return OUT_OF_MEM;
            }
            TInt32 rtn = m_pNet->Connect(m_ipMaps.m_connectedNodeInfo.m_realIp,m_ipMaps.m_pVirtualIps[m_ipMaps.m_localVirtualIp].m_realIp,
                m_ipMaps.m_connectedNodeInfo.m_listenPort,m_ipMaps.m_pVirtualIps[m_ipMaps.m_localVirtualIp].m_bindPort,p);
            if (rtn < SUCCESS)
            {
#ifdef _DEBUG
                printf("Connection Failed!");
#endif
                m_connectionPool.ReleaseItem(p);
                return rtn;
            }
            m_ppConnections[m_ipMaps.m_redirectIdx] = p;
            break;
        }
    }

    //读取配置，看有几个Service, 需要启动几个工作Service.
    CSettingFile settingFile;
    TInt32 nrOfComm = 2;
    TInt32 inPipeSize = 512*1024;
    TInt32 outPipeSize = 512*1024;
    TInt32 maxMsgSize = 256*1024;
    if (!settingFile.Load("commSetting.ini"))
    {

    }
    else
    {
        nrOfComm = settingFile.GetInteger("MAIN","nrOfComm",nrOfComm);
        inPipeSize = settingFile.GetInteger("MAIN","inPipeSize",inPipeSize);
        outPipeSize = settingFile.GetInteger("MAIN","outPipeSize",outPipeSize);
        maxMsgSize = settingFile.GetInteger("MAIN","maxMsgSize",maxMsgSize);
    }
    NEW(m_pBuff,TUChar,maxMsgSize);
    if (!m_pBuff)
    {
#ifdef _DEBUG
        printf("Can not allocated memory for m_pBuff");
#endif
        return OUT_OF_MEM;
    }
    
    m_nrOfComm = nrOfComm;
    NEW(m_pCommunicators,CCommunicator,nrOfComm);
    if (!m_pCommunicators)
    {
#ifdef _DEBUG
        printf("Allocate mem 4 m_pCommunicators Failed!");
#endif
        //OnFinal();
        return OUT_OF_MEM;
    }
    
    for (int i=0;i<nrOfComm;++i)
    {
        int ret = m_pCommunicators[i].Init(inPipeSize,outPipeSize,maxMsgSize);
        if (ret < SUCCESS)
        {
#ifdef _DEBUG
            printf("Can not init Communicator %d",i);
#endif
            return ret;
        }
    }
    //再看是否需要主动连接外node.
    return SUCCESS;
}

IfCommunicator *CCommMgr::RegisterWorker(TUInt16 srvId)
{
    int idx = srvId % m_nrOfComm;
    return m_pCommunicators + idx;
}

TInt32 CCommMgr::Run(const TInt32 threadId,const TInt32 runCnt)
{
    int usedCnt = 0;
    //先调整时间
    m_timeSystem.Update();
    
    //再收消息，底层buff小
    usedCnt += m_pNet->Run(runCnt);
    //再转发消息
    for (int i = 0;i<m_nrOfComm;++i)
    {
 //       usedCnt += DistributeSrvMsg(i);
        CMessageHeader *pMsg = m_pCommunicators[i].GetAppMsg(m_pBuff);
        if(CheckNetState(pMsg))
        {
            SendAppMsg(pMsg);
            if (m_pBuff != (TUChar *)pMsg)
            {
                 m_pCommunicators[i].AppMsgSent(pMsg);   
            }
        }
    }
    return usedCnt;
}

TBool CCommMgr::CheckNetState(CMessageHeader *pMsg)
{
    //计算需要发给几个ip,首先node不同
    int nrOfDest = pMsg->GetBroadcastDoidNr();
    //有广播
    if (nrOfDest)
    { 
        CDoid *pDoid = pMsg->GetDestDoidByIdx();
        int idx = m_ipMaps.GetConnectionIdx(pDoid);
        m_ipMaps.GetConnectionIdx(pDoid);
        int nrToSend = 1;
        int i = 1;
        while(i <= nrOfDest)
        {
            CDoid *pNext = pMsg->GetDestDoidByIdx(i);
            if (pDoid->IsInOneIp(*pNext))
            {
                ++i;
                ++nrToSend;
            }
            else
            {
                //不同了
                TUInt32 needLen = pMsg->GetBodyLength() + sizeof(CMessageHeader) + (sizeof(CDoid) * (nrToSend));
                //小于0，则丢弃.
                if (idx > 0)
                {
                    //connection没连上，那就不管了.
                    if (m_ppConnections[idx])
                    {
                        if (m_ppConnections[idx]->GetFreeBuffLength() < needLen)
                        {
                            return FALSE;
                        }
                    }
                }
                pDoid = pNext;
                idx = m_ipMaps.GetConnectionIdx(pDoid);
                nrToSend = 1;
                ++i;
            }
        }

        TUInt32 needLen = pMsg->GetBodyLength() + (sizeof(CMessageHeader) + sizeof(CDoid) * (nrToSend));
        //小于0，则丢弃.
        if (idx > 0)
        {
            //connection没连上，那就不管了.
            if (m_ppConnections[idx])
            {
                if (m_ppConnections[idx]->GetFreeBuffLength() < needLen)
                {
                    return FALSE;
                }
            }
        }
    }
    else //没有广播
    {
        int idx = m_ipMaps.GetConnectionIdx(pMsg->GetDestDoidByIdx());
        if (idx >= 0)
        {
            TUInt32 freeLen = m_ppConnections[idx]->GetFreeBuffLength();
            if (pMsg->GetLength() <= freeLen)
            {
                return TRUE;
            }
        }
        return FALSE;
    }
}


void CCommMgr::SendAppMsg(CMessageHeader *pMsg)
{
    //计算需要发给几个ip,首先node不同
    int nrOfDest = pMsg->GetBroadcastDoidNr();
    //有广播
    if (nrOfDest)
    { 
        CDoid *pDoid = pMsg->GetDestDoidByIdx();
        int idx = m_ipMaps.GetConnectionIdx(pDoid);
        m_ipMaps.GetConnectionIdx(pDoid);
        int from = 0;
        int i = 1;
        while(i <= nrOfDest)
        {
            CDoid *pNext = pMsg->GetDestDoidByIdx(i);
            if (pDoid->IsInOneIp(*pNext))
            {
                ++i;
            }
            else
            {
                //不同了
               /* TUInt32 needLen = pMsg->GetBodyLength() + (sizeof(CMessageHeader) * (nrToSend));*/
                //小于0，则丢弃.
                pMsg->ReInitMsg4Send(from,(i-1));
                if (idx > 0)
                {
                    //connection没连上，那就不管了.
                    if (m_ppConnections[idx])
                    {
                        m_ppConnections[idx]->SendMsg((TUChar*)pMsg,pMsg->GetLength());
                    }
                }
                else
                {
                    //发送失败
                }
                pDoid = pNext;
                idx = m_ipMaps.GetConnectionIdx(pDoid);
                from = i;
                ++i;
            }
        }
        
        
        if (idx > 0)
        {
            //connection没连上，那就不管了.
            pMsg->ReInitMsg4Send(from,nrOfDest);
            if (m_ppConnections[idx])
            {
                m_ppConnections[idx]->SendMsg((TUChar*)pMsg,pMsg->GetLength());
            }
            else//发送失败
            {

            }
        }
        pMsg->SetBroadcastDoid(nrOfDest);
    }
    else //没有广播
    {
        int idx = m_ipMaps.GetConnectionIdx(pMsg->GetDestDoidByIdx());
        if (idx >= 0)
        {
            m_ppConnections[idx]->SendMsg((TUChar*)pMsg,pMsg->GetLength());
        }
        //return FALSE;
    }

}

}
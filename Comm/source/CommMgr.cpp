#include "CommMgr.h"
#include "SettingFile.h"
#include "../Public/include/NetCenter.h"
#include "../System/include\SysInc.h"
#include "include/CommLogger.h"
namespace Zephyr
{


TInt32 CCommMgr::Init(int nrOfWorkerThread,IfTaskMgr *pTaskMgr,IfLoggerManager *pIfLogMgr,const TChar *pConfigName/* =szDefaultLoggerName */)
{
    //初始化网络底层
    
    //先读取配置
    TInt32 ret = m_ipMaps.Init(pConfigName,this);
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
    int nRet = m_connectionPool.InitPool(m_ipMaps.m_nrOfVirtualIp + m_ipMaps.m_nrOfNodes);
    if (nRet)
    {

    }
//     NEW(m_ppConnections,CCommConnection*,m_ipMaps.m_nrOfVirtualIp + 5);
//     if (!m_ppConnections)
//     {
// #ifdef _DEBUG
//         printf("Can not allocate memory for m_ppConnections");
// #endif
//         return OUT_OF_MEM;
//     }
    //开始监听一个port

	//主动连接vip比自己小的所有机器，每台机器重启后都是这个顺序.并且只尝试重连比自己vip小的机器
    //if (m_ipMaps.m_nrOfNodes > 1)
    {
        for (int i = 0;i<m_ipMaps.m_nNrOfMapItem;++i)
        {
            if (m_ipMaps.IsPostive(i))
            {
                CCommConnection *pConnection = m_connectionPool.GetMem();
                if (!pConnection)
                {
#ifdef _DEBUG
                    printf("Can not get Comm Connection");
#endif
                    return OUT_OF_MEM;
                }
                CIpMapItem *pIp = m_ipMaps.GetConnection(i);
                TInt32 rtn = m_pNet->Connect(pIp->m_tKey.m_realIp,m_ipMaps.m_pVirtualIps[m_ipMaps.m_localVirtualIp].m_tKey.m_realIp,
                    pIp->m_tKey.m_listenPort,pIp->m_tKey.m_bindPort,pConnection);
                if (rtn < SUCCESS)
                {
#ifdef _DEBUG
                    printf("Connection Failed!");
#endif
                    m_connectionPool.ReleaseMem(pConnection);
                    return rtn;
                }
                //m_ppConnections[i] = pConnection;
                pConnection->SetAllInfo(this,pIp);
                pIp->OnConnecting(pConnection,m_timeSystem.GetLocalTime());
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
    }
    

    //读取配置，看有几个Service, 需要启动几个工作Service.
    CSettingFile settingFile;
    TInt32 inPipeSize = 512*1024;
    TInt32 outPipeSize = 512*1024;
    TInt32 maxMsgSize = 256*1024;
    if (!settingFile.LoadFromFile("commSetting.ini"))
    {

    }
    else
    {
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
    
    m_nrOfComm = nrOfWorkerThread;
    NEW(m_pCommunicators,CCommunicator,nrOfWorkerThread);
    if (!m_pCommunicators)
    {
#ifdef _DEBUG
        printf("Allocate mem 4 m_pCommunicators Failed!");
#endif
        //OnFinal();
        return OUT_OF_MEM;
    }
    
    for (int i=0;i<nrOfWorkerThread;++i)
    {
        int ret = m_pCommunicators[i].Init(&m_timeSystem,inPipeSize,outPipeSize,maxMsgSize);
        m_pCommunicators[i].InitEventPool(m_ipMaps.m_nrOfVirtualIp);
        if (ret < SUCCESS)
        {
#ifdef _DEBUG
            printf("Can not init Communicator %d",i);
#endif
            return ret;
        }
    }
    //再看是否需要主动连接外node.
    m_pLoggerMgr = pIfLogMgr;
    ret = m_pLoggerMgr->AddLogger("CommLogger",-1);
    if (ret < SUCCESS)
    {
        return ret;
    }
    m_pLogger = m_pLoggerMgr->GetLogger(ret);
    g_pCommLogger = m_pLogger;
    pTaskMgr->AddTask(this);
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
        if (!pMsg)
        {
            continue;
        }
        //if(CheckNetState(pMsg))
        {
            SendAppMsg(pMsg);
            if (m_pBuff != (TUChar *)pMsg)
            {
                 m_pCommunicators[i].AppMsgSent(pMsg);   
            }
            ++usedCnt;
        }
    }
    //一分钟检查一次
    TUInt32 uTimeNow = m_timeSystem.GetLocalTime() - 60000;
    for (TUInt32 i = 0;i<m_ipMaps.m_nNrOfMapItem;++i)
    {
        CIpMapItem *pItem = m_ipMaps.GetConnection(i);
        if (pItem->m_uLastUsedTime < uTimeNow)
        {
            if (pItem->m_pConnection)
            {
                //发送心跳消息
            }
            else
            {
                //重连
            }
        }
    }
    return usedCnt;
}

TBool CCommMgr::CheckNetState(CMessageHeader *pMsg)
{
    //无论如何都成功
    return TRUE;
    //计算需要发给几个ip,首先node不同
//     int nrOfDest = pMsg->GetBroadcastDoidNr();
//     //有广播
//     if (nrOfDest)
//     { 
//         CDoid *pDoid = pMsg->GetDestDoidByIdx();
//         //int idx = m_ipMaps.GetConnectionIdx(pDoid);
//         IfConnection *pConn = m_ipMaps.RouteTo(pDoid);
//         int nrToSend = 1;
//         int i = 1;
//         while(i <= nrOfDest)
//         {
//             CDoid *pNext = pMsg->GetDestDoidByIdx(i);
//             if (pDoid->IsInOneIp(*pNext))
//             {
//                 ++i;
//                 ++nrToSend;
//             }
//             else
//             {
//                 //不同了
//                 TUInt32 needLen = pMsg->GetBodyLength() + sizeof(CMessageHeader) + (sizeof(CDoid) * (nrToSend));
//                 //小于0，则丢弃.
//                 //if (idx > 0)
//                 {
//                     //connection没连上，那就不管了.
//                     if (pConn)
//                     {
//                         if (pConn->GetFreeBuffLength() < needLen)
//                         {
//                             return FALSE;
//                         }
//                     }
//                 }
//                 pDoid = pNext;
//                 pConn = m_ipMaps.RouteTo(pDoid);
//                 nrToSend = 1;
//                 ++i;
//             }
//         }
// 
//         TUInt32 needLen = pMsg->GetBodyLength() + (sizeof(CMessageHeader) + sizeof(CDoid) * (nrToSend));
//         //小于0，则丢弃.
//         //if (idx > 0)
//         {
//             //connection没连上，那就不管了.
//             if (pConn)
//             {
//                 if (pConn->GetFreeBuffLength() < needLen)
//                 {
//                     return FALSE;
//                 }
//             }
//         }
//     }
//     else //没有广播
//     {
//         //int idx = m_ipMaps.GetConnectionIdx(pMsg->GetDestDoidByIdx());
//         IfConnection *pConn = m_ipMaps.RouteTo(pMsg->GetDestDoidByIdx(0));
//         if (pConn)
//         {
//             TUInt32 freeLen = pConn->GetFreeBuffLength();
//             if (pMsg->GetLength() <= freeLen)
//             {
//                 return TRUE;
//             }
//         }
//         return FALSE;
//     }
}


void CCommMgr::SendAppMsg(CMessageHeader *pMsg)
{
    //计算需要发给几个ip,首先node不同
    int nrOfDest = pMsg->GetBroadcastDoidNr();
    //有广播
    if (nrOfDest)
    { 
        CDoid *pDoid = pMsg->GetDestDoidByIdx();
        IfConnection *pConn = m_ipMaps.RouteTo(pDoid);
        int from = 0;
        int i = 1;
        while(i <= nrOfDest)
        {
            CDoid *pNext = pMsg->GetDestDoidByIdx(i);

            int nRet = FAIL;
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
                //if (idx > 0)
                {
                    //connection没连上，那就不管了.
                    if (pConn)
                    {
                        nRet = pConn->SendMsg((TUChar*)pMsg,pMsg->GetLength());
                    }
                }
                if (nRet < SUCCESS)
                {
                    RecordOneMsg(pMsg);
                }
                pDoid = pNext;
                pConn = m_ipMaps.RouteTo(pDoid);
                from = i;
                ++i;
                
            }
        }
        
        int nRet = FAIL;
        if (pConn)
        {
            if (from)
            {
                pMsg->ReInitMsg4Send(from,nrOfDest);
            }
            //if (m_ppConnections[idx])
            {
                nRet = pConn->SendMsg((TUChar*)pMsg,pMsg->GetLength());
            }
        }
        if (nRet < SUCCESS) //connection没连上,记录消息
        {
            RecordOneMsg(pMsg);
        }
        pMsg->SetBroadcastDoid(nrOfDest);
    }
    else //没有广播
    {
        IfConnection *pConn  = m_ipMaps.RouteTo(pMsg->GetDestDoidByIdx());
        int nRet = FAIL;
        if (pConn)
        {
            nRet = pConn->SendMsg((TUChar*)pMsg,pMsg->GetLength());
        }
        if (nRet < SUCCESS)
        {
            RecordOneMsg(pMsg);
        }
        //return FALSE;
    }

}

//收到的时候必须都是CMessagerHeader的格式，由调用者保证
TInt32 CCommMgr::SendMsg(TUChar *pBuff,TUInt32 buffLen)
{
    CMessageHeader *pMsg = (CMessageHeader*)pBuff;
    TUInt32 usedLen = 0;
    TUInt32 msgLen = pMsg->GetLength();
    while (msgLen >= buffLen)
    {
        usedLen += msgLen;
        buffLen -= msgLen;
        HandleOneNetMsg(pMsg);
        if (sizeof(CMessageHeader) < buffLen)
        {
            pMsg = (CMessageHeader*)(pBuff + usedLen);
        }
        else
        {
            break;
        }
    }
    return usedLen;
}
//永远是最大
TUInt32 CCommMgr::GetFreeBuffLength()
{
    return 0x7FFFFFFF;
}
//返回本地连接本地的
//获取连接信息
CConPair *CCommMgr::GetConnectionInfo()
{
    return &m_cLoopBack;;
}
//设置是否需要Negla算法，无用
TInt32 CCommMgr::NeedNoDelay(const char chOpt)
{
    return SUCCESS;
}
//获取连接状态，无用
EnConnectionState CCommMgr::GetConnctionState()
{
    return connection_is_using;
}
//用以获取还未发送的数据的长度，无用
TInt32 CCommMgr::GetPendingDataLen()
{
    return 0;
}

//对CCommMgr来说，这个无用
TInt32 CCommMgr::Disconnect()
{
    return SUCCESS;
}

CIpMapItem *CCommMgr::GetIpMapInfo(CConPair *pPair)
{
    return m_ipMaps.GetIpMapInfo(pPair);
}

void   CCommMgr::OnConnected(CCommConnection *pConnection)
{
    CCommConnection *pLast = m_ipMaps.OnConnected(pConnection,m_timeSystem.GetLocalTime());
    if (pLast != pConnection)
    {
        pLast->Disconnect();
        m_connectionPool.ReleaseMem(pLast);
    }
}
void   CCommMgr::OnDisconnected(CCommConnection *pConnection)
{
    m_ipMaps.OnDisconnected(pConnection,m_timeSystem.GetLocalTime());
    m_connectionPool.ReleaseMem(pConnection);
}


IfConnectionCallBack *CCommMgr::OnNewConnection(CConPair *pPair)
{
    TUInt16 uNode,uVip;
    CIpMapItem *pItem = GetIpMapInfo(pPair);
    if (!pItem)
    {
        return NULL;
    }
    //
    CCommConnection *pConn = m_connectionPool.GetMem();
    pConn->SetAllInfo(this,pItem);
    return pConn;
}


void CCommMgr::HandleOneNetMsg(CMessageHeader *pMsg)
{
    int i = 0;
    int from = 0;
    int to = 1;
    CDoid *pDoid = pMsg->GetDestDoidByIdx(i);
    CCommunicator *pComm = GetIfComm(pDoid->m_srvId);
    int msgDoidNr = pMsg->GetBroadcastDoidNr();
    if (msgDoidNr)
    {
        pDoid = pMsg->GetBroadcastDoids();
        for (i=1;i<= msgDoidNr;++i)
        {
            if (pComm != GetIfComm(pDoid->m_srvId))
            {
                //send last msg
                to = i;
                pMsg->ReInitMsg4Send(from,to);
                pComm->AddNetMsg(pMsg);
                pMsg->SetBroadcastDoid(msgDoidNr);
                from = to;
                pComm = GetIfComm(pDoid->m_srvId);
            }
            else
            {
                //pComm = GetIfComm(pDoid->m_srvId);
            }
            ++pDoid;
        }
    }
    //如果from改变过，则需要重新init Msg.
    if (from)
    {
        pMsg->ReInitMsg4Send(from,(msgDoidNr+1));
    }
    pComm->AddNetMsg(pMsg);
    pMsg->SetBroadcastDoid(msgDoidNr);
}

}
#include <iostream>
#include "ConnectionMgr.h"
#include "TaskMgr.h"
using namespace std;
using namespace Zephyr;

class CNetApp : public IfNetApp
{
public:
    unsigned long long m_recvedMsg;
    unsigned long m_unReplyed;
    int m_nrOfConnection;
    unsigned long *m_pMsgCnt;
    unsigned long *m_pMsgSendCnt;
    long long *m_pConnectionRecvied;
    long long *m_pConnectionSent;
    long      *m_pConnectionUnReplied;
    unsigned int *m_pPositiveSendCnt;
    CConnectionMgr *m_pConnMgr;
    unsigned int    m_positiveSendTimes;
    unsigned char *m_pMsg;
public:
    CNetApp(int connectionNr)
    {
        m_pMsg = new unsigned char[128];
        m_positiveSendTimes = 1;
        for (int i=0;i<128;i++)
        {
            m_pMsg[i] = i;
        }
        m_recvedMsg = 0;
        m_unReplyed = 0;

        m_nrOfConnection = connectionNr;
        m_pConnectionRecvied = new long long[connectionNr];
        m_pConnectionSent    = new long long[connectionNr];
        m_pConnectionUnReplied = new long[connectionNr];
        m_pMsgCnt              = new unsigned long[connectionNr];
        m_pMsgSendCnt          = new unsigned long[connectionNr];
        m_pPositiveSendCnt     = new unsigned int[connectionNr];
        for(int i =0;i<connectionNr;i++)
        {
            m_pConnectionRecvied[i] = 0;
            m_pConnectionSent[i] = 0;
            m_pMsgCnt[i] = 0;
            m_pMsgSendCnt[i] = 0;
            m_pConnectionUnReplied[i] = -1;
            m_pPositiveSendCnt[i] = 0;
        }
    }
    void SetPositiveSendTime(unsigned int time)
    {
        m_positiveSendTimes = time;
    }
    bool NeedPositiveSend(int idx)
    {
        if (m_pPositiveSendCnt[idx] < m_positiveSendTimes)
        {
            return true;
        }
        return false;
    }
    void OnPositiveSendSuccess(int idx)
    {
        m_pPositiveSendCnt[idx] ++;
    }
    bool CheckSeq(int idx)
    {
        int delta = m_pMsgSendCnt[idx] - m_pMsgCnt[idx];
        if (delta <0)
        {
            delta = -delta;
        }
        if (delta > 3)
        {
            return false;
        }
        return true;
    }
    void SetConnMgr(CConnectionMgr *pConnMgr)
    {
        m_pConnMgr = pConnMgr;
    }
    bool IsConnected(int idx)
    {
        if (m_pConnectionUnReplied[idx] != -1)
        {
            return true;
        }
        return false;
    }
    int GetUnRepliedMsg(int idx)
    {
        if (IsConnected(idx))
        {
            return m_pConnectionUnReplied[idx];
        }
        return -1;
    }
    bool IsJustConnected(int idx)
    {
        if (m_pConnectionSent[idx] == 0)
        {
            return true;
        }
        return false;
    }
    void ConfirmSend(int idx,int len)
    {
        m_pConnectionSent[idx] += len;
        m_pMsgSendCnt[idx] ++;
    }
    void ConfirmReplyMsg(int idx,int len)
    {
        if (IsConnected(idx))
        {
            m_pConnectionSent[idx] += len;
            m_pConnectionUnReplied[idx] -= len;
            if (m_pConnectionUnReplied[idx] < 0)
            {
                m_pConnectionUnReplied[idx] = 128;
            }
            m_pMsgSendCnt[idx] ++;
        }
        else
        {
            m_pMsgSendCnt[idx] ++;
        }
    }
    virtual TInt32 OnRecv(TUInt32 connectionIdx,TUChar *pMsg, TUInt32 msgLen,void *pAppData)
    {
        //cout<<"Recv "<<msgLen<<"bytes from connection:"<<connectionIdx<<endl;
        m_pConnectionRecvied[connectionIdx] += msgLen;
        m_pConnectionUnReplied[connectionIdx] += msgLen;
        m_pMsgCnt[connectionIdx] ++;
        return msgLen;
    }
    virtual TInt32 OnRecvIn2Piece(TUInt32 connectionIdx,TUChar *pMsg, TUInt32 msgLen,TUChar *pMsg2,TUInt32 msgLen2)
    {
        //cout<<"Recv "<<msgLen<<"bytes from connection:"<<connectionIdx<<endl;
        m_pConnectionRecvied[connectionIdx] += msgLen;

        m_pConnectionUnReplied[connectionIdx] += msgLen;
        m_pConnectionRecvied[connectionIdx] += msgLen2;
        m_pConnectionUnReplied[connectionIdx] += msgLen2;
        m_pMsgCnt[connectionIdx] ++;

        return (msgLen + msgLen2);
    }
    virtual TInt32 OnConnected(TUInt32 connectionIdx,void *pAppData)
    {
        cout<<"Connection "<<connectionIdx<<"is established!"<<endl;
        //m_connectionIdx = connectionIdx;

        if (connectionIdx < m_nrOfConnection)
        {
            m_pConnectionUnReplied[connectionIdx] = 0;
            //m_pConnMgr->SendMsg(connectionIdx,m_pMsg,128);
        }
        return SUCCESS;
    }
    virtual TInt32 OnDissconneted(TUInt32 connectionIdx,void *pUserData)
    {
        cout<<"Connection "<<connectionIdx<<"is broken!"<<endl;
        if (connectionIdx < m_nrOfConnection)
        {
            m_pConnectionUnReplied[connectionIdx] = -1;
        }
        return SUCCESS;
    }
    virtual TInt32 OnNetError(TUInt32 connectionIdx,void *pUserData)
    {
        cout<<"Connection"<<connectionIdx<<"has error:"<<endl;
        if (connectionIdx < m_nrOfConnection)
        {
            m_pConnectionUnReplied[connectionIdx] = -1;
        }
        return SUCCESS;
    }
};


int main()
{
    int connectionNr = 0;
    cout<<"Input nr of connection:"<<endl;
    cin>>connectionNr;
    CNetApp netApp(connectionNr);
    CTaskMgr taskMgr;   
    CConnectionMgr *pConnectionMgr = new CConnectionMgr(&netApp,&taskMgr);
    pConnectionMgr->Init(connectionNr);
    char myIp[32];
    cout<<"Input local Ip:";
    cin>>myIp;
    unsigned short listeningPort,bindPort;
    cout<<"Input listeningPort:";
    cin>>listeningPort;
    cout<<"Input bindPort:";
    cin>>bindPort;
    unsigned long ip = inet_addr(myIp);
    pConnectionMgr->Listen(ip,listeningPort,10,&netApp);
    netApp.SetConnMgr(pConnectionMgr);
    taskMgr.StartWorking(8);
    cout<<"need connect?"<<endl;
    char yes;
    cin>>yes;
    if ((yes == 'y')||(yes == 'Y'))
    {
        int t = 1;
        cout<<"Input positive send times"<<endl;
        cin>>t;
        netApp.SetPositiveSendTime(t);
        char remoteIp[32];
        cout<<"Input remote Ip"<<endl;
        cin>>remoteIp;
        cout<<"Input remote Port:";
        unsigned short remotePort;
        cin>>remotePort;
        for (int i =0;i<connectionNr;i++)
        {
            pConnectionMgr->Connect(remoteIp,myIp,remotePort,(bindPort+i),&netApp);
            if (0 == ((i+1)%32))
            {
                for(int j=0;j<40;j++)
                {
                    pConnectionMgr->Run(128);
                    Sleep(25);
                }
            }
        }
    }
    
    unsigned char msg[256];
    for(int i=0;i<256;i++)
    {
        msg[i] = i;
    }
    int forcedSleep = 0;
    int needReply = 1;
    while(1)
    {
        int runCnt = pConnectionMgr->Run(256);
        int ret = 0;
        /*if (netApp.m_unReplyed> 128)
        {
            pConnectionMgr->SendMsg(netApp.m_connectionIdx,msg,128);
            netApp.m_unReplyed -=128;
        }*/
        for (int i=0;i<connectionNr;i++)
        {
            if (netApp.IsConnected(i))
            {
                netApp.CheckSeq(i);
                int unRepliedLength = netApp.GetUnRepliedMsg(i);
                if ((unRepliedLength > 0) && (needReply))
                {
                    if (unRepliedLength > 256)
                    {
                        unRepliedLength = 128;
                    }
                    if (unRepliedLength > 128)
                    {
                        unRepliedLength = 128;
                    }
                    int ret = pConnectionMgr->SendMsg(i,msg,unRepliedLength);
                    if (ret > 0)
                    {
                        netApp.ConfirmReplyMsg(i,ret);
                    }
                }
                
                if(netApp.NeedPositiveSend(i))
                {
                    int ret = pConnectionMgr->SendMsg(i,msg,128);
                    if(ret > 0)
                    {
                        netApp.OnPositiveSendSuccess(i);
                        netApp.ConfirmSend(i,ret);
                    }
                }
                else
                {
                    int j=0;
                    j++;
                }
                //int sentBytes = netApp.GEt
            }
            

        }
        if ((!runCnt))
        {
            forcedSleep ++;
        //Sleep(15);
            if (0==(forcedSleep % 16))
            {
                Sleep(15);
            }
            //Sleep(0);
        }
        //Sleep(15);
        /*if (0==(forcedSleep % 2))
        {
            Sleep(15);
        }*/
        
    }
    return 0;
}



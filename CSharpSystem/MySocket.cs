using UnityEngine;
using System.Collections;
using System.Threading;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;
using System;



public enum EnNetState
{
    start_run,
    connecting,
    connect_fail,
    connected,
    disconeected
}

public class MySocket : MonoBehaviour
{
    public UIButton m_pButton;
    public UIButton m_pSendButton;
    
	private class Worker
	{
        private Queue m_tSendQueue = Queue.Synchronized(new Queue());
        private Queue m_tRecvQueue = Queue.Synchronized(new Queue());
		private CMessage m_pRecving;
		private IPEndPoint m_tSvrIp;
		public EnNetState m_enStatus;
		private Socket m_tSocket;
        private byte[] m_pRecvBuffer = new byte[MacrosAndDef.MSG_HEADER_LEN];
		private int m_nRecvSeak;
		private bool m_bIsSending;
        public bool m_bRuning = true;
        MySocket m_pOwner;
		public Worker(string szSvrAddr,int svrPort,MySocket pOwner)
		{
            m_pOwner = pOwner;
            m_enStatus = EnNetState.start_run;
			IPAddress tIpAddr = IPAddress.Parse(szSvrAddr);
			m_tSvrIp = new IPEndPoint(tIpAddr,svrPort);
            m_tSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            m_tSocket.Blocking = true;//
            try
            {
                Debug.Log("Begin Connect!");
                m_tSocket.BeginConnect(m_tSvrIp, new AsyncCallback(ConnectCallback), m_tSocket); 
            }
            catch (Exception se)
            {
                m_enStatus = EnNetState.connect_fail;
                Debug.Log("Connecting failed!");
                return;
            }

            m_enStatus = EnNetState.connecting;

            ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProc));
		}
        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                client.EndConnect(ar);
                //if connection is failed, client will throw an exceptions.
                Debug.Log("Socket connected to "+client.RemoteEndPoint.ToString());
                m_enStatus = EnNetState.connected;
                CMessage.sm_pSocket = m_pOwner;
                /*m_tSocket.Send()*/
            }
            catch (Exception e)
            {
                OnNetException(e.ToString());
            }
        }

        public void SendMsg(CMessage pMsg)
        {
            Debug.Log("SendMsg is called!");
            m_tSendQueue.Enqueue(pMsg);
            StartSend();
        }

        public CMessage RecvMsg()
        {
            Debug.Log("RecvMsg is called!");
            if (m_tRecvQueue.Count > 0)
            {
                return (CMessage)m_tRecvQueue.Dequeue();
            }
            return null;
        }

        private void StartSend()
        {
            Debug.Log("StartSend is called!");
            if (EnNetState.connected == m_enStatus)
            {
                lock (this)
                {
                    if (false == m_bIsSending)
                    {
                        if (m_tSendQueue.Count > 0)
                        {
                            CMessage pMsg = (CMessage)m_tSendQueue.Dequeue();
                            if (null != pMsg)
                            {
                                m_bIsSending = true;
                                m_tSocket.BeginSend(pMsg.m_pBuffers, 0, pMsg.m_pBuffers.Length, 0, new AsyncCallback(OnSent), pMsg);
                            }
                        }
                    }
                }
            }
        }

        private void StartRecv()
        {
            if (EnNetState.connected == m_enStatus)
            {
                Debug.Log("StartRecv is called!");
                m_tSocket.BeginReceive(m_pRecvBuffer, m_nRecvSeak, m_pRecvBuffer.Length-m_nRecvSeak, 0, new AsyncCallback(OnRecv), m_pRecvBuffer);
            }
        }


        void OnNetException(string str)
        {
            Debug.Log("OnNetException is called!");
            m_enStatus = EnNetState.connect_fail;
            Debug.Log(str);
        }

        private void OnSent(IAsyncResult ar)
        {
            Debug.Log("OnSent is called!");
            try
            {
                m_tSocket.EndSend(ar);
                m_bIsSending = false;
                StartSend();
            }
            catch(Exception e)
            {
                OnNetException(e.ToString());
            }
        }
        private void OnDisconnected()
        {
            m_enStatus = EnNetState.disconeected;
            m_tSocket = null;
            CMessage.sm_pSocket = null;
        }
        private void OnRecv(IAsyncResult ar)
        {
            Debug.Log("OnRecv is called!");
            try
            {
                int nLength = m_tSocket.EndReceive(ar);
                //
                Debug.Log("Recv " + nLength + " Bytes!");
            }
            catch(Exception e)
            {
                m_enStatus = EnNetState.disconeected;

                OnNetException(e.ToString());
            }

            if (m_bRuning)
            {
                ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProc));
            }
        }

		private void ThreadProc(object obj)
		{
            Debug.Log("ThreadProc is called!");
            //byte[] pszBuffer = new byte[1];
            switch (m_enStatus)
            {
                case EnNetState.connected:
                    {
                        StartRecv();
                    }
                    break;
                //                 case EnNetState.start_run:
                //                      {
                //                          //do nothing
                //                      }
                //                      break;
                //                 case EnNetState.connecting:
                //                      {
                //                          //waiting!
                //                      }
                //                      break;
                //                 case EnNetState.connect_fail:
                //                      {
                //                          //wait;
                //                      }
                //                      break;
                //                 case EnNetState.disconeected:
                //                      {
                //                         //do something !
                //                      }
                //                      break;
                default:
                    {
                        //Thread.Sleep(15);
                    }
                    break;
            }
            //
		}
	}
    private Worker m_pWorker;
	// Use this for initialization
	void Start () 
    {
        Debug.Log("Start is called!");
        m_pButton.SetValueChangedDelegate(OnButtonClicked);
        m_pSendButton.SetValueChangedDelegate(TestIf);
        CMessage.sm_pSocket = this;
	}
   
    public void OnButtonClicked(IUIObject obj)
    {
        Debug.Log("OnButtonClicked is called!");
        if (null == m_pWorker)
        {
            Debug.Log("Start new Worker!");
            m_pWorker = new Worker("127.0.0.1", 12222,this);
        }
    }
    public void TestIf(IUIObject obj)
    {
        if (null != m_pWorker)
        {
            Debug.Log("Start test if!");
            if (EnNetState.connected == m_pWorker.m_enStatus)
            {
                Debug.Log("Connection is established and we will send the msg!");
                IfTest pIfTest = new IfTestStub();
                pIfTest.Test(1);
            }
        }
    }

    private int StartLogin(string szIp,int nPort)
    {
        Debug.Log("StartLogin is called!");
        m_pWorker = new Worker(szIp, nPort,this);
        if (null != m_pWorker)
        {
            return (int)m_pWorker.m_enStatus;
        }
        return - 1;
    }
    public int SendMsg(CMessage pMsg)
    {
        Debug.Log("SendMsg is called!");
        m_pWorker.SendMsg(pMsg);
        return 0;
    }
    public CMessage GetMsg()
    {
        return m_pWorker.RecvMsg();
    }
    void OnDisable()
    {
        Debug.LogError("Ondestoryed!");
        if (null != m_pWorker)
        {
            m_pWorker.m_bRuning = false;
            m_pWorker = null;
        }
    }
}





public class CMessage
{
    public static MySocket sm_pSocket;
    public int m_uMsgLength;
    public uint m_uMsgId;
    public byte[] m_pBuffers;
    public CMessage(uint uMsgId, int uMsgLength)
    {
        m_uMsgId = uMsgId;
        m_uMsgLength = uMsgLength;
        try
        {
            m_pBuffers = new byte[uMsgLength + MacrosAndDef.MSG_HEADER_LEN];
        }
        catch(Exception e)
        {

        }
    }
    public void UnmarshallHeader()
    {
        if (m_pBuffers.Length > MacrosAndDef.MSG_HEADER_LEN)
        {
            TypeUnmarshaller.Unmarshall(m_pBuffers, m_pBuffers.Length, 0, out m_uMsgLength);
            TypeUnmarshaller.Unmarshall(m_pBuffers, m_pBuffers.Length, sizeof(int),out m_uMsgId);
        }
    }
}

interface IfTest
{
    int Test(int nType);
}

class IfTestMethodId
{
    public const uint IfTestTestMethondId = 20;
}

class IfTestStub : IfTest
{
    int IfTest.Test(int nType)
    {
        Debug.Log("IfTest.Test(int nType)");
        int nLength = TypeMarshaller.GetLength(nType);
        CMessage pMsg = new CMessage(IfTestMethodId.IfTestTestMethondId, nLength);
        if (null == pMsg.m_pBuffers)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        int nBufferLen = pMsg.m_pBuffers.Length;
        int nUsed = 0;

        nUsed = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, nLength);
        nUsed += TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, pMsg.m_uMsgId);
        int nLen = TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, nUsed, nType);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return nLen;
        }
        nUsed += nLen;
        if (nUsed != nBufferLen)
        {
            Debug.LogError("the nUsed is:"+nUsed);
            TypeMarshaller.Marshall(pMsg.m_pBuffers, nBufferLen, sizeof(uint), nUsed);
        }
        //send on net.
        CMessage.sm_pSocket.SendMsg(pMsg);
        return nUsed;
    }
}

class CSkeleton
{
    virtual public int  HandleMsg(CMessage pMsg)
    {
        return MacrosAndDef.MSG_NOT_HANDLED; 
    }
}

class IfTestSkeleton : CSkeleton
{
    IfTest m_pImplementObj;
    public IfTestSkeleton(IfTest pIf)
    {
        m_pImplementObj = pIf;
    }
    public override int HandleMsg(CMessage pMsg)
    {
        pMsg.UnmarshallHeader();
        switch (pMsg.m_uMsgId)
        {
            case IfTestMethodId.IfTestTestMethondId:
                {
                    return IfTestTest_int(pMsg);
                }
                break;
        }
        return MacrosAndDef.MSG_NOT_HANDLED;
    }
    int IfTestTest_int(CMessage pMsg)
    {
        int nUsed = MacrosAndDef.MSG_HEADER_LEN;
        int nBufferLen = pMsg.m_pBuffers.Length;
        int _nType=0;
        int nLen = TypeUnmarshaller.Unmarshall(pMsg.m_pBuffers, nBufferLen, nUsed, out _nType);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return m_pImplementObj.Test(_nType);
    }
}
#include "DBLib.h"


namespace DBLib
{


CDBTransationWorkThread::CDBTransationWorkThread(CDBTransationManager * pManager)
{
	m_pConnection=NULL;
	m_pManager=pManager;
}

CDBTransationWorkThread::~CDBTransationWorkThread(void)
{

	Destory();
}

void CDBTransationWorkThread::Destory()
{
	CDBTransaction * pDBTansaction=NULL;
	while(m_TransQueue.PopFront(pDBTansaction))
	{
		SAFE_DELETE(pDBTansaction);
	}
	while(m_FinishTransQueue.PopFront(pDBTansaction))
	{
		SAFE_DELETE(pDBTansaction);
	}
	SAFE_DELETE(m_pConnection);
}

bool CDBTransationWorkThread::Init(IDBConnection * pConnection,LPCTSTR ConnectStr,int QueueSize)
{
	if(pConnection==NULL)
		return false;

	Destory();

	strncpy_0(m_ConnectString,ConnectStr,MAX_CONNECT_STRING);;
	m_pConnection=pConnection;
	m_pConnection->EnableTransaction(true);
	m_TransQueue.Create(QueueSize);
	m_FinishTransQueue.Create(QueueSize);
	m_ConnectionTestTimer.SaveTime();

	PrintDBLog(0xff,"工作线程队列长度%d",QueueSize);

	return (Start() == SUCCESS);
}

bool CDBTransationWorkThread::AddTransaction(CDBTransaction * pDBTansaction)
{
	if(pDBTansaction)
		return m_TransQueue.PushBack(pDBTansaction);
	else
		return false;
}

CDBTransaction * CDBTransationWorkThread::PopFinishTransaction()
{
	CDBTransaction * pDBTansaction=NULL;
	m_FinishTransQueue.PopFront(pDBTansaction);
	return pDBTansaction;
}

BOOL CDBTransationWorkThread::OnStart()
{
	return SUCCESS;
}
BOOL CDBTransationWorkThread::OnRun()
{
	CDBTransaction * pDBTansaction=NULL;
	//LPVOID Pos=m_TransQueue.GetFirstObjectPos();
	m_TransQueue.PopFront(pDBTansaction);
	if(pDBTansaction)
	{
		LONG ExecTime=CEasyTimer::GetTime();
		bool Ret=pDBTansaction->OnExecute(m_pConnection);
		ExecTime=GetTimeToTime(ExecTime,CEasyTimer::GetTime());
		m_pManager->AddExecTime(ExecTime);
		if(Ret)
		{
			m_pConnection->Commit();
		}
		else
		{
			PrintDBLog(0xff,"数据库事务执行出错，回滚");
			m_pConnection->RollBack();
		}
		pDBTansaction->SetResult(Ret);
		m_FinishTransQueue.PushBack(pDBTansaction);
		//m_TransQueue.PopFront(pDBTansaction);
	}
	else
	{
		SleepT(1);
	}
	if(m_ConnectionTestTimer.IsTimeOut(CONNECTION_TEST_TIME))
	{
		if(!m_pConnection->IsConnected())
		{
			PrintDBLog(0xff,"连接已断开，重新连接");
			m_pConnection->Disconnect();
			m_pConnection->Connect((LPCTSTR)m_ConnectString);
            m_pConnection->EnableTransaction(true);
		}
		m_ConnectionTestTimer.SaveTime();
	}
	return SUCCESS;
}
void CDBTransationWorkThread::OnBeginTerminate()
{

}

}

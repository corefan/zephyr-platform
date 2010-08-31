#include "NetTask.h"
#include "WinBase.h"

namespace Zephyr
{



TInt32 CNetTask::Init(TInt32 maxNrOfConnection,CConnection *pConnections,void *completionPort)
{
    m_completionPort = completionPort;
    m_pConnections = pConnections;
    m_maxConnectionNr = maxNrOfConnection;
#ifdef _DEBUG
    m_startTime = timeGetTime();
    m_runCnt = 0;
    m_averageTime = 0;
#endif
	//closesocket( s );
	return SUCCESS;
}
TInt32 CNetTask::Begin(TInt32 threadId)
{
    if (NULL == m_completionPort)
    {
        return NOT_INITIALIZED;
    }
    return SUCCESS;
}
TInt32 CNetTask::Run(const TInt32 threadId,const TInt32 runCnt)
{
#ifdef _DEBUG
    TUInt32 timeNow = timeGetTime();
    m_runCnt ++;
    if (!m_runCnt)
    {
        m_runCnt = 1;
    }
    m_averageTime = (timeNow - m_startTime)/m_runCnt;
#endif
    TInt32 usedCnt = 0;
    //TUInt32 gap = 0;
    //GET_TIME_2_TIME(gap,m_appUsingOutQueue,m_workerUsingOutQueue);
    ////handle the new
    //if (gap)
    //{
    //    TplNode<CNetEvent,TUInt16>::Iterator itor;
    //    itor.Init(m_pAppEvent[(m_workerUsingOutQueue%NR_OF_QUEUE_IN_TASK)]);
    //    TInt32 size = itor.Size();
    //    for (TInt32 i=0;i<size;i++)
    //    {
    //        
    //    }
    //}
    for (int i = 0;i<m_maxConnectionNr;i++)
    {
        usedCnt += m_pConnections[i].Routine();
    }

    while (runCnt > usedCnt)
    {
		DWORD dwIoSize;
		CConnection* pConnection;
		LPOVERLAPPED lpOverlapped;
		
        BOOL bIORet = GetQueuedCompletionStatus(
				m_completionPort,
				&dwIoSize,
				(LPDWORD) &pConnection,
				&lpOverlapped, SELECT_TIME_OUT);

			// If Something whent wrong..
        if (!bIORet)  
        {
			DWORD dwIOError = GetLastError();
			
			if(dwIOError != WAIT_TIMEOUT) // It was not an Time out event we wait for ever (INFINITE) 
			{	
				//TRACE(_T("GetQueuedCompletionStatus errorCode: %i, %s\n"),WSAGetLastError(),pThis->ErrorCode2Text(dwIOError));			
				// if we have a pointer & This is not an shut down.. 
				//if (lpClientContext!=NULL && pThis->m_bShutDown == false)
				if (pConnection!=NULL)
				{
					/*
					* ERROR_NETNAME_DELETED Happens when the communication socket
					* is cancelled and you have pendling WSASend/WSARead that are not finished. 
					* Then the Pendling I/O (WSASend/WSARead etc..) is cancelled and we return with 
					* ERROR_NETNAME_DELETED.. 
					*/
					if(dwIOError== ERROR_NETNAME_DELETED) 
					{
						//连接关闭
						if(lpOverlapped!=NULL)
						{
						    CIocpOverlappedDataHeader *pHeader = CONTAINING_RECORD(lpOverlapped, CIocpOverlappedDataHeader, m_ol);
						    if (pConnection == pHeader->m_pConnection)
						    {
						        OnDisconnected(pConnection);
						    }
						    else
						    {
						        //something wrong!
						    }
                        }

					}
                    else
                    { 
						//啥错了？ 
                    }
					// Clear the buffer if returned. 
					usedCnt ++;
					continue;
				}
				usedCnt ++;
				continue;
			}
            else
            {
                //that means the network is idle. So let the cpu sleep for a while;
                return usedCnt;
            }
        }


        if(bIORet && lpOverlapped && pConnection) 
        {
            CIocpOverlappedDataHeader *pHeader = CONTAINING_RECORD(lpOverlapped, CIocpOverlappedDataHeader, m_ol);
            if(pHeader->m_pConnection == pConnection)
            {
                usedCnt += ProcessIOMessage(pHeader,dwIoSize);
            }
            else
            {
                //something wrong!
            }
            continue;
        }
        else
        {
            //something wrong again!
        }
        usedCnt ++;
        continue;
    }
    return usedCnt;
}
TInt32 CNetTask::End(TInt32 threadId)
{
    return SUCCESS;
}

//
//inline TInt32 CNetTask::AddNetEvent(CConnection *pConnection,TUInt16 eventType)
//{
//    /*TUInt32 idx = (m_workerUsingOutQueue % NR_OF_QUEUE_IN_TASK);
//    TplNode<CNetEvent,TUInt16> *pEvent = (m_pWorkerEventPool[idx] + (pConnection->GetConnectionIdx()));
//    #ifdef _DEBUG
//    TplNode<CNetEvent,TUInt16> *pExist = m_pNetTaskEvent[idx]->FindNode(pEvent->m_key);
//    if (pExist)
//    {
//        if (!pEvent->m_isUsed)
//        {
//            return CONNECTION_IN_WRONG_STATE;
//        }
//    }
//    #endif
//    if (!pEvent->m_isUsed)
//    {
//        pEvent->m_key       = pConnection->GetConnectionIdx();
//        pEvent->m_isUsed    = 1;
//        pEvent->m_seqNum    = pConnection->GetSeqNum();
//        pEvent->m_eventType = eventType;
//        pEvent->Init();
//        if (m_pNetTaskEvent[idx])
//        {
//            m_pNetTaskEvent[idx] = m_pNetTaskEvent[idx]->AddNode(pEvent);
//        }
//        else
//        {
//            m_pNetTaskEvent[idx] = pEvent;
//        }
//    }
//    else
//    {
//        pEvent->m_eventType |= eventType;
//    }*/
//    return SUCCESS;
//}


TInt32 CNetTask::OnDisconnected(CConnection *pConnection)
{
    pConnection->Disconnect();
    m_connectionAborted ++;
    return SUCCESS;
    //return AddNetEvent(pConnection,NET_CONNECTION_BROKEN);
}




TInt32 CNetTask::ProcessIOMessage(CIocpOverlappedDataHeader *pHeader,TUInt32 ioSize)
{
    TInt32 usedCnt = 0;
    switch (pHeader->m_operation)
    {
        case overlapped_not_in_using:
        {
            //something wrong!
            if (pHeader->m_pConnection)
            {
                
            }
        }
        break;
        case overlapped_try_reading:
        {
            if (TRUE == pHeader->m_pConnection->CheckReadHeader(pHeader))
            {
                usedCnt += pHeader->m_pConnection->OnRecv(pHeader,ioSize);
                //OnRecv(pHeader->m_pConnection);
                
            }
            else
            {
                //pHeader->FreeHeader();
            }
        }
        break;
        case overlapped_try_reading_with_zero_mem:
        {
            if (TRUE == pHeader->m_pConnection->CheckReadHeader(pHeader))
            {
                usedCnt += pHeader->m_pConnection->OnRecv(pHeader,ioSize);
            }
            else
            {
                //pHeader=>FreeHeader();
            }
        }
        break;
        case overlapped_try_writing:
        {
            if (TRUE == pHeader->m_pConnection->CheckWriteHeader(pHeader))
            {
                usedCnt += pHeader->m_pConnection->OnSent(pHeader,ioSize);
            }
            else
            {
            
            }
        }
        default:
        {
            //something wrong!
        }
    }
    return usedCnt;
}


//
//TInt32 CNetTask::Send(CIocpOverlappedDataHeader *pHeader)
//{
//    DWORD dwSendNumBytes = 0;
//    ULONG ulFlags = 0;
//    int result = WSASend(pHeader->m_pConnection->GetSocket(),
//                        &(pHeader->m_wsaBuff),
//                        1,
//                        &dwSendNumBytes, 
//                        ulFlags,
//                        &pHeader->m_ol, 
//                        NULL
//                        );
//    if(result)
//    {
//        int err = WSAGetLastError();
//        if (WSA_IO_PENDING == err)
//        {
//            //this is ok!
//        }
//        else if (WSAENOBUFS == err)
//        {
//            
//        }
//    }
//    else
//    {
//        //every thing is correct!
//    }
//    return SUCCESS;
//}


}

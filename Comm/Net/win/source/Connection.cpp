
#include "Connection.h"

#include "SysMacros.h"

namespace Zephyr
{



TInt32  CConnection::AppSend(TUChar *pData,TUInt32 dataLen)
{
    return m_outPipe.WriteData(pData, dataLen);
}

//TUChar *Peek(TUChar *pData,TUInt32 needLength);
TInt32 CConnection::OnRecv(CIocpOverlappedDataHeader *pHeader,TUInt32 ioSize)
{
    #ifdef _DEBUG
    if (pHeader != &m_readDataHeader)
    {
        m_errorCode = CONNECTION_DATA_INCORRECT;
        Disconnect();
        return NOT_BELONG_TO_THIS_CAPSULA;
    }
    /*if (ioSize > 256)
    {
        return -1;
    }*/
    #endif
    if (pHeader->m_operation != overlapped_try_reading_with_zero_mem)
    {
        if (0 == ioSize) // connection is droped!
        {
            m_errorCode = CONNECTION_DATA_INCORRECT;
            Disconnect();
            return 1;
        }
        TInt32 result = m_inPipe.ConfirmAddMsg((TUChar*)pHeader->m_wsaBuff.buf, ioSize);
        #ifdef _DEBUG
        m_msgRecv += ioSize;
        #endif
#ifdef _DEBUG
        //TInt32 usedSize = m_inPipe.GetDataLen();
        /*if (usedSize > 256)
        {
            usedSize +=3;
        }*/
#endif	
        if (SUCCESS != result)
        {
            return result;
        }
    }
    
    
    m_readDataHeader.FreeHeader();
    CIocpOverlappedDataHeader *pData = NULL;
    pData = PrepareToRead();
    //if (m_rLock.TryLock())
    //{
    //    
    //    //m_connectionState = connection_is_using;
    //    m_rLock.Unlock();
    //}
    ULONG ulFlags = 0;
    if (pData)
    {

        DWORD dwIoSize=0;
        UINT nRetVal = WSARecv(m_socket,
            &pData->m_wsaBuff,
            1,
            &dwIoSize, 
            &ulFlags,
            &pData->m_ol,
            NULL);

        if (nRetVal == SOCKET_ERROR)
        {
            int err = WSAGetLastError();
            if (err != WSA_IO_PENDING)
            {
                if (err == WSAENOBUFS)
                {
                    //Send a zero byte read.
                    //pData->m_wsaBuff.len = 0;
                    pData->PrepZeroByteRead();
                    UINT nRetVal = WSARecv(m_socket,
                        &pData->m_wsaBuff,
                        1,
                        &dwIoSize, 
                        &ulFlags,
                        &pData->m_ol,
                        NULL);
                    if (nRetVal == SOCKET_ERROR)
                    {
                        int err = WSAGetLastError();
                        if (err != WSA_IO_PENDING)
                        {
                            Disconnect();
                        }
                    }
                    return 1;
                }
                Disconnect();
                //connection broken;
                return 1;
            }
        }
    }
	return 1;
	//TUInt32 remainLen = m_inPipe.GetDataLen();
}
TInt32 CConnection::OnSent(CIocpOverlappedDataHeader *pHeader,TUInt32 ioSize)
{
	#ifdef _DEBUG
	if (pHeader != & m_writeDataHeader)
    {
        m_errorCode = CONNECTION_DATA_INCORRECT;
        m_connectionState = connection_is_not_in_use;
        Disconnect();
        return NOT_BELONG_TO_THIS_CAPSULA;
    }
	#endif
	TInt32 result = m_outPipe.ReturnMsgBuff((TUChar*)pHeader->m_wsaBuff.buf, ioSize);
	#ifdef _DEBUG
	m_msgSend += ioSize;
	#endif
	//m_connectionState = connection_is_not_in_use;
	if (SUCCESS != result)
	{
	    return result;
	}
    m_writeDataHeader.FreeHeader();
    CIocpOverlappedDataHeader *pData = NULL;
    pData = GetWritingData();
    /*if (m_wLock.TryLock())
    {
        pData = GetWritingData();
        m_wLock.Unlock();
    }*/
    if (pData)
    {
        DWORD dwSendNumBytes = 0;
        ULONG ulFlags = 0;
        int nRetVal = WSASend(m_socket, 
            &pData->m_wsaBuff,
            1,
            &dwSendNumBytes, 
            ulFlags,
            &pData->m_ol, 
            NULL);
        if (nRetVal == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                Disconnect();
                return 1;
                //connection broken;
            }
        }
    }
	return SUCCESS;
}

void CConnection::Callback(IfNetApp *pApp)
{
    int piece = m_inPipe.GetDataPiece();
    if (piece == 1)
    {
        TUInt32 len = m_inPipe.GetDataLen();
        TUChar *pMsg = m_inPipe.GetData(len);
        m_pConfirmedTo = pMsg + len;
        int ret = pApp->OnRecv(m_connectionIdx,pMsg,len,m_pAppData);
        if ((ret > 0) && (ret <= len))
        {
            m_inPipe.ReturnMsgBuff(pMsg,ret);
        }
        else
        {
            m_inPipe.ReturnMsgBuff(pMsg,len);
        }
    }
    else if(piece == 2)
    {
        TUInt32 totalLen = m_inPipe.GetDataLen();
        TUInt32 len = totalLen;
        TUChar *pMsg = m_inPipe.GetData(len);
        
        TUInt32 sndLen;
        TUChar *pSnd = m_inPipe.GetSndPiece(sndLen);
        #ifdef _DEBUG
        if ((len + sndLen) !=totalLen)
        {
        #endif
            totalLen = len + sndLen;
        #ifdef _DEBUG
        }
        #endif
        m_pConfirmedTo = pSnd + sndLen;
        int ret = pApp->OnRecvIn2Piece(m_connectionIdx,pMsg,len,pSnd,sndLen);
        if ((ret > 0) & (ret <= (totalLen)))
        {
            if (ret < len)
            {
                m_inPipe.ReturnMsgBuff(pMsg,ret);
            }
            else
            {
                m_inPipe.ReturnMsgBuff(pMsg,len);
                m_inPipe.ReturnMsgBuff(pSnd,(ret - len));
            }
            
        }
    }
    #ifdef _DEBUG
    else
    {
    }
    #endif
}


TInt32 CConnection::OnCreate(TUInt32 idx,TUInt32 pipeSize)
{
    m_connectionIdx = idx;
    m_seqNum = 0;
    TInt32 ret = m_inPipe.Init(pipeSize);
    if (ret < SUCCESS)
    {
        return ret;
    }
    ret = m_outPipe.Init(pipeSize);
    m_pConfirmedTo = m_inPipe.GetDataHeader();
    return ret;
}

//尝试读取所有可以读取的数据
CIocpOverlappedDataHeader *CConnection::GetWritingData()
{
    if (m_writeDataHeader.IsFree())
    {
        TUInt32 wDataLen = m_outPipe.GetDataLen();
        if (wDataLen)
        {
            
            m_writeDataHeader.m_wsaBuff.buf = (char*)m_outPipe.GetData(wDataLen);
            if (wDataLen > MAX_IOCP_PACKET_DATA_LENGTH)
            {
                wDataLen = MAX_IOCP_PACKET_DATA_LENGTH;
            }
            m_writeDataHeader.m_wsaBuff.len = wDataLen;
            m_writeDataHeader.m_operation = overlapped_try_writing;
            return &m_writeDataHeader;
        }
    }
    return NULL;
}
CIocpOverlappedDataHeader *CConnection::PrepareToRead()
{
    if (m_readDataHeader.IsFree())
    {
        TInt32 len = m_inPipe.GetFreeLen();
        if (!len)
        {
            return NULL;
        }
        /*if(len > MAX_IOCP_PACKET_DATA_LENGTH)
        {
            len = MAX_IOCP_PACKET_DATA_LENGTH;
        }*/
        TUInt32 availableLen = len;
        TUChar *pBuff = m_inPipe.PrepareMsg(availableLen);
        if(!availableLen)
        {
            return NULL;
        }
        if(availableLen > MAX_IOCP_PACKET_DATA_LENGTH)
        {
            availableLen = MAX_IOCP_PACKET_DATA_LENGTH;
        }
        if (pBuff)
        {
            m_readDataHeader.m_wsaBuff.buf = (char*)pBuff;
            m_readDataHeader.m_wsaBuff.len = availableLen;
            m_readDataHeader.m_operation = overlapped_try_reading;
            #ifdef _DEBUG
            if (!availableLen)
            {
                m_readDataHeader.m_operation = overlapped_try_reading_with_zero_mem;
            }
            #endif
        }
        else
        {
            m_readDataHeader.m_wsaBuff.buf = (char*)(&m_errorCode);
            m_readDataHeader.m_wsaBuff.len = 0;
            m_readDataHeader.m_operation = overlapped_try_reading_with_zero_mem;
        }
        return &m_readDataHeader;
    }
    return NULL;
}


void CConnection::Disconnect()
{
    CancelIo((HANDLE)m_socket);
    int ret = closesocket(m_socket);
    ret = WSAGetLastError();
    m_seqNum ++;
    m_connectionState = connection_is_broken;
    m_socket = NULL;
}

TInt32 CConnection::OnInit()
{
    m_inPipe.Reset();
    m_outPipe.Reset();
    m_errorCode = 0;
    m_connectionState = connection_is_not_in_use;
    m_readDataHeader.Init(this,m_seqNum);
    m_writeDataHeader.Init(this,m_seqNum);
    #ifdef _DEBUG
    m_msgSend = 0;
    m_msgRecv = 0;
    #endif
    
    return SUCCESS;
}

TInt32 CConnection::OnFinal()
{
    m_connectionState = connection_is_not_in_use;
    return SUCCESS;
}

void CConnection::ConnectionIsTrying()
{
    m_connectionState = connection_is_trying;
}



void CConnection::ConnectionIsBroken()
{
    m_connectionState = connection_is_broken;
}

TInt32 CConnection::SetSocketOnAccept(SOCKET sock,CConnectionMgr *pMgr)
{
    sockaddr_in remoteAdd,myAdd;
    int     remoteNameLen = sizeof(sockaddr_in);
    int     myNameLen = remoteNameLen;
    int ret = getpeername(sock,(sockaddr *)&remoteAdd,&remoteNameLen);
    if (ret == SOCKET_ERROR)
    {
        return (-((TInt32)WSAGetLastError()));
    }
    
    ret = getsockname(sock,(sockaddr *)&myAdd,&myNameLen);
    m_socket = sock;
    //myAdd.sin_addr.s_addr
    
    m_key.Init(remoteAdd.sin_addr.s_addr,myAdd.sin_addr.s_addr,ntohs(remoteAdd.sin_port),ntohs(myAdd.sin_port));
    m_pMgr = pMgr;
    return SUCCESS;
    //m_key.Init(TUInt32 remoteIp, TUInt32 myIp, TUInt16 remotePort, TUInt16 myPort)
    //m_key.Init(TUInt32 remoteIp, TUInt32 myIp, TUInt16 remotePort, TUInt16 myPort)
}

TInt32 CConnection::Init(TUInt32 remoteIp,TUInt32 myIp, TUInt16 remotePort,TUInt16 myPort,TUInt32 connectionType,void *pAppData,CConnectionMgr *pMgr)
{
    m_key.Init(remoteIp,myIp,remotePort,myPort);
    m_seqNum = 0;
    m_connectionType = connectionType;
    m_pAppData = pAppData;
    m_pMgr = pMgr;
    return SUCCESS;
}

TInt32 CConnection::SetSocketOptions()
{
    LINGER lingerStruct;
    lingerStruct.l_onoff = 1;
    lingerStruct.l_linger = 1;
    //use api's orignal types
    int ret = setsockopt(m_socket, SOL_SOCKET, SO_LINGER,
              (char *)&lingerStruct, sizeof(lingerStruct));
    if (ret == SOCKET_ERROR)
    {
        int errcode = WSAGetLastError();
        return -errcode;
    }
   /* 
    * TCP_NODELAY	 BOOL=TRUE Disables the "nagle algorithm for send coalescing" which delays
    * short packets in the hope that the application will send more data and allow
    * it to combine them into a single one to improve network efficiency.
    */
    const char chOpt = 1;
    ret = setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
    if (ret == SOCKET_ERROR)
    {
        int errcode = WSAGetLastError();
        return -errcode;
    }
    unsigned long argp   =   1;   
    ret   =   ioctlsocket(m_socket,FIONBIO,(unsigned   long*)&argp);
    if(ret   ==   SOCKET_ERROR)   
    {   
        int errcode = WSAGetLastError();
        return -errcode;
    }
    
    return SUCCESS;
}


TInt32 CConnection::Routine()
{
    switch(m_connectionState)
    {
    case connection_is_not_in_use:  //do nothing.
        {
            return 0;
        }
        break;
    case connection_is_trying: //do nothing .
        {
            return 0;
        }
        break;
    case connection_is_established: //app confirmed. 
        {
            //prepare to recv data.
            TInt32 usedCnt = 0;
            CIocpOverlappedDataHeader *pData = NULL;
            if (m_rLock.TryLock())
            {
                pData = PrepareToRead();
                m_connectionState = connection_is_using;
                m_rLock.Unlock();
            }
            ULONG ulFlags = 0;
            if (pData)
            {
                usedCnt ++;
                DWORD dwIoSize=0;
                UINT nRetVal = WSARecv(m_socket,
					&pData->m_wsaBuff,
					1,
					&dwIoSize, 
					&ulFlags,
					&pData->m_ol,
					NULL);
					
				if (nRetVal == SOCKET_ERROR)
				{
				    int err = WSAGetLastError();
				    if (err != WSA_IO_PENDING)
				    {
				        if (err == WSAENOBUFS)
				        {
                            //Send a zero byte read.
                            //pData->m_wsaBuff.len = 0;
                            pData->PrepZeroByteRead();
                            UINT nRetVal = WSARecv(m_socket,
					                        &pData->m_wsaBuff,
					                        1,
					                        &dwIoSize, 
					                        &ulFlags,
					                        &pData->m_ol,
					                        NULL);
                            if (nRetVal == SOCKET_ERROR)
                            {
                                int err = WSAGetLastError();
                                if (err != WSA_IO_PENDING)
                                {
                                    Disconnect();
                                }
                            }
				            return 1;
				        }
                        Disconnect();
				        //connection broken;
				        return 1;
				    }
				}
            }
            pData = NULL;
            if (m_wLock.TryLock())
            {
                pData = GetWritingData();
                m_wLock.Unlock();
            }
            if (pData)
            {
                usedCnt ++;
                //send the data.
                //
                DWORD dwSendNumBytes = 0;
                
                int nRetVal = WSASend(m_socket, 
				&pData->m_wsaBuff,
				1,
				&dwSendNumBytes, 
				ulFlags,
				&pData->m_ol, 
				NULL);
				if (nRetVal == SOCKET_ERROR)
				{
				    if (WSAGetLastError() != WSA_IO_PENDING)
				    {
                        Disconnect();
                        return 1;
				    //connection broken;
				    }
				}
            }
            return usedCnt;
        }
        break;
    case connection_is_using: //do the routines 
        {
            CIocpOverlappedDataHeader *pData = NULL;
            /*if (m_wLock.TryLock())
            {
                
                m_wLock.Unlock();
            }*/
            pData = GetWritingData();
            if (pData)
            {
                //send the data.
                //
                DWORD dwSendNumBytes = 0;
                ULONG ulFlags = 0;
                int nRetVal = WSASend(m_socket, 
				&pData->m_wsaBuff,
				1,
				&dwSendNumBytes, 
				ulFlags,
				&pData->m_ol, 
				NULL);
				if (nRetVal == SOCKET_ERROR)
				{
				    if (WSAGetLastError() != WSA_IO_PENDING)
				    {
                        Disconnect();
				    //connection broken;
				    }
				}
            }
            pData = PrepareToRead();
            ULONG ulFlags = 0;
            if (pData)
            {

                DWORD dwIoSize=0;
                UINT nRetVal = WSARecv(m_socket,
                    &pData->m_wsaBuff,
                    1,
                    &dwIoSize, 
                    &ulFlags,
                    &pData->m_ol,
                    NULL);

                if (nRetVal == SOCKET_ERROR)
                {
                    int err = WSAGetLastError();
                    if (err != WSA_IO_PENDING)
                    {
                        if (err == WSAENOBUFS)
                        {
                            //Send a zero byte read.
                            //pData->m_wsaBuff.len = 0;
                            pData->PrepZeroByteRead();
                            UINT nRetVal = WSARecv(m_socket,
                                &pData->m_wsaBuff,
                                1,
                                &dwIoSize, 
                                &ulFlags,
                                &pData->m_ol,
                                NULL);
                            if (nRetVal == SOCKET_ERROR)
                            {
                                int err = WSAGetLastError();
                                if (err != WSA_IO_PENDING)
                                {
                                    Disconnect();
                                }
                            }
                            return 1;
                        }
                        Disconnect();
                        //connection broken;
                        return 1;
                    }
                }
            }
        }
        break;
    case connection_is_broken: //do nothing. wait the app to release it.
        {
            //do nothing.
        }
        break;
    default:
        {
        
        }
    }
    return 0;

}

}


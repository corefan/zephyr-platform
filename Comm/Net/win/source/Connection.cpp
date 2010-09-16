#include "Connection.h"
#include "ConnectionMgr.h"
#include "SysMacros.h"

namespace Zephyr
{



TInt32  CConnection::SendMsg(TUChar *pData,TUInt32 dataLen)
{
    //the net has consume the previews evnet! 
    //先写数据
    TInt32 result = m_outPipe.WriteData(pData, dataLen);
    //再写event
    if (SUCCESS == result)
    {
        OnAppSend();
    }
    return result;
}

//TUChar *Peek(TUChar *pData,TUInt32 needLength);
TInt32 CConnection::OnRecv(CIocpOverlappedDataHeader *pHeader,TUInt32 ioSize)
{
    #ifdef _DEBUG
    if (pHeader != &m_readDataHeader)
    {
        m_errorCode = CONNECTION_DATA_INCORRECT;
        //Disconnect();
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
            OnNetDisconnected();
            return 1;
        }
        if (m_pIfCryptor)
        {
            m_pIfCryptor->Decrypt((TUChar *)pHeader->m_wsaBuff.buf, ioSize);
        }
        TInt32 result = m_inPipe.ConfirmAddMsg((TUChar*)pHeader->m_wsaBuff.buf, ioSize);
        OnNetRecv();
        #ifdef _DEBUG
        m_msgRecv += ioSize;
        if (SUCCESS != result)
        {
            return result;
        }
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
            m_errorCode = WSAGetLastError();
            if (m_errorCode != WSA_IO_PENDING)
            {
                if (m_errorCode == WSAENOBUFS)
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
                        m_errorCode = WSAGetLastError();
                        if (m_errorCode != WSA_IO_PENDING)
                        {
                            CloseConnection();
                            OnNetDisconnected();
                        }
                    }
                    return 1;
                }
                CloseConnection();
                OnNetDisconnected();
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
        //m_connectionState = connection_is_not_in_use;
        //Disconnect();
        
        //if(connection_is_using == m_connectionState)
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
                CloseConnection();
                return 1;
                //connection broken;
            }
        }
    }
    else
    {
        CheckAppDisconnected();
    }
	return SUCCESS;
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
    //m_pConfirmedTo = m_inPipe.GetDataHeader();
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

TInt32 CConnection::Disconnect()
{
    switch (m_connectionState)
    {
        case connection_is_not_in_use:
        {
            return NOT_INITIALIZED;
        }
        break;
        case connection_is_trying:
        {
            //立即释放自己，不应该.
            return NOT_INITIALIZED;
        }
        break;
        case connection_is_established:
        case connection_is_broken:
        case connection_is_aborted:
        {
            m_pAppCallBack = NULL;
            m_appDisconnect = 1;
        }
        break;
    }
    return SUCCESS;
}

CConPair *CConnection::GetConnectionInfo()
{
    return &m_key;
}
TInt32 CConnection::NeedNoDelay(const char chOpt) 
{
    return DisableNagle(chOpt);
}

EnConnectionState CConnection::GetConnctionState()
{
    return (EnConnectionState)m_connectionState;
}

void CConnection::CloseConnection()
{
    //TInt32 ret = SUCCESS;
    //默认成功,这儿不得不加个锁，以防应用层和网络层同时关闭socket,其实这也没啥事，closesocket是线程安全的
    //if(m_disconnectLock.TryLock())
    {
        if (NULL != m_socket)
        {
            TInt32 ret = closesocket(m_socket);
            //无论如何都关闭.
            //if (SUCCESS == ret)
            {
                m_socket = NULL;
                //m_connectionState = connection_is_aborted;
            }
            m_seqNum ++;
        }
        //m_disconnectLock.Unlock();
    }
    //return ret;
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
    m_netDisconnect = 0;
    m_appDisconnect = 0;
    m_connectionType = 0;
    return SUCCESS;
}

TInt32 CConnection::OnFinal()
{
    m_connectionState = connection_is_not_in_use;
    m_netDisconnect = 0;
    m_appDisconnect = 0;
    m_connectionType = 0;
    return SUCCESS;
}

//
TInt32 CConnection::SetSocketOnAccept(SOCKET sock,CConnectionMgr *pMgr)
{
    
    return SUCCESS;
    //m_key.Init(TUInt32 remoteIp, TUInt32 myIp, TUInt16 remotePort, TUInt16 myPort)
    //m_key.Init(TUInt32 remoteIp, TUInt32 myIp, TUInt16 remotePort, TUInt16 myPort)
}

TInt32 CConnection::Init(SOCKET socket,CConPair *pPair,void *pAppCallBack,void *pParser,void* pCryptor/* =NULL */)
{
    m_socket = socket;
    TInt32 ret = SetSocketOptions();
    if (SUCCESS > ret)
    {
        return ret;
    }
    //sockaddr_in remoteAdd,myAdd;
    //int     remoteNameLen = sizeof(sockaddr_in);
    //int     myNameLen = remoteNameLen;
    //int ret = getpeername(socket,(sockaddr *)&remoteAdd,&remoteNameLen);
    //if (ret == SOCKET_ERROR)
    //{
    //    return (-((TInt32)WSAGetLastError()));
    //}

    //ret = getsockname(socket,(sockaddr *)&myAdd,&myNameLen);
    ////myAdd.sin_addr.s_addr

    //m_key.Init(remoteAdd.sin_addr.s_addr,myAdd.sin_addr.s_addr,ntohs(remoteAdd.sin_port),ntohs(myAdd.sin_port));
    m_key = *pPair;
    m_pAppCallBack = (IfConnectionCallBack*)pAppCallBack;
    m_pIfParser    = (IfParser*)pParser; 
    m_pIfCryptor   = (IfCryptor*)pCryptor;
    
}

TInt32 CConnection::SetSocketOptions()
{
    LINGER lingerStruct;
    lingerStruct.l_onoff = 0;
    lingerStruct.l_linger = 0;
    //generally close the socket,and do not block the app.
    //use api's orignal types
    int ret = setsockopt(m_socket, SOL_SOCKET, SO_LINGER,
              (char *)&lingerStruct, sizeof(lingerStruct));
    if (ret == SOCKET_ERROR)
    {
        int errcode = WSAGetLastError();
        return -errcode;
    }

    //-------------------------
    // Set the socket I/O mode: In this case FIONBIO
    // enables or disables the blocking mode for the 
    // socket based on the numerical value of iMode.
    // If argp = 0, blocking is enabled; 
    // If argp != 0, non-blocking mode is enabled.

    unsigned long argp   =   1;   
    ret   =   ioctlsocket(m_socket,FIONBIO,(unsigned   long*)&argp);
    if(ret   ==   SOCKET_ERROR)   
    {   
        int errcode = WSAGetLastError();
        return -errcode;
    }
    
    return SUCCESS;
}

//由应用层调用，如果返回-1，则表示需要释放连接,把链接close,并且放回connectionPool
TInt32 CConnection::AppRoutine(TUChar *pBuff,TUInt32 buffLen)
{
    if (m_connectionState == connection_is_using)
    {
        if (!m_pAppCallBack)
        {
            TUInt32 len = m_inPipe.GetDataLen();
            while (len)
            {
                //TUInt32 avail = len;
                TUChar *pData = m_inPipe.GetData(len);
                m_inPipe.ReturnMsgBuff(pData,len);
                len = m_inPipe.GetDataLen();
            }
            return -1;
        }
        TUInt32 len = m_inPipe.GetDataLen();
        TUChar *pHeader(NULL);
        while(len > m_minHeaderLength)
        {
            if (m_pIfParser)
            {
                TUInt32 headerLen(0);
                TInt32 dataLen(0);
                if (len > m_maxHeaderLength)
                {
                    headerLen = m_maxHeaderLength;
                    pHeader = m_inPipe.GetData(headerLen);
                    if (headerLen < m_maxHeaderLength)
                    {
                        pHeader = m_inPipe.Peek(pBuff,m_maxHeaderLength);
                        dataLen = m_maxHeaderLength;
                    }
                    else
                    {
                        dataLen = headerLen;
                    }
                }
                else
                {
                    headerLen = m_minHeaderLength;
                    pHeader = m_inPipe.GetData(headerLen);
                    if (headerLen < m_minHeaderLength)
                    {
                        pHeader = m_inPipe.Peek(pBuff,m_minHeaderLength);
                        dataLen = m_minHeaderLength;
                    }
                    else
                    {
                        dataLen = headerLen;
                    }
                    //headerLen = m_minHeaderLength;
                }
                //if(tmp)
                
                    //保存需要的长度.
                dataLen = m_pIfParser->OnRecv(pHeader,dataLen);

                if (dataLen < len)
                {
                    TUInt32 needLen = dataLen;
                    pHeader = m_inPipe.GetData(needLen);
                    if (needLen < dataLen)
                    {
                        m_inPipe.ReadData(pBuff,dataLen);
                    }
                    else
                    {
                        if (m_pAppCallBack)
                        {
                            m_pAppCallBack->OnRecv(pBuff,dataLen);
                        }
                        m_inPipe.ReturnMsgBuff(pHeader,dataLen);
                    }
                }
                else
                {
                    break;
                }
               
            }
            else
            {
                pHeader = m_inPipe.GetData(len);
                if (m_pAppCallBack)
                {
                    m_pAppCallBack->OnRecv(pHeader,len);
                }
                m_inPipe.ReturnMsgBuff(pHeader,len);
            }
            len = m_inPipe.GetDataLen();
        }
    }
    else if (connection_is_aborted == m_connectionState)
    {
        return -1;
    }
    else if (connection_is_broken == m_connectionState)
    {
        if (m_pAppCallBack)
        {
            m_pAppCallBack->OnDissconneted(m_errorCode);
            return -1;
        }
    }
}
//由NetTask掉也难怪，如果返回-1，
TInt32 CConnection::NetRoutine()
{
    if (m_connectionState == connection_is_using)
    {
        CIocpOverlappedDataHeader* pData = GetWritingData();
        if (m_pIfCryptor)
        {
            m_pIfCryptor->Encrypt((TUChar *)pData->m_wsaBuff.buf,m_writeDataHeader.m_wsaBuff.len);
        }
        if (pData)
        {
            //send the data.
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
            return 1;
        }
        else
        {
            CheckAppDisconnected();
        }
    }
    return 0;
}

void CConnection::CheckAppDisconnected()
{
    if (m_appDisconnect)
    {
        m_connectionState = connection_is_aborted;
        //关闭连接
        CloseConnection();
    }
}


TInt32 CConnection::OnConnected()
{
    //CIocpOverlappedDataHeader *pData = PrepareToRead();
    TInt32 ret = m_pAppCallBack->OnConnected(this,m_pIfParser,m_pIfCryptor);
    if (SUCCESS != ret)
    {
        //CloseConnection();
        m_appDisconnect = 1;
        return ret;
    }
    CIocpOverlappedDataHeader *pData = PrepareToRead();
    
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
            m_errorCode = WSAGetLastError();
            if (m_errorCode != WSA_IO_PENDING)
            {
                if (m_errorCode == WSAENOBUFS)
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
                        m_errorCode = WSAGetLastError();
                        if (m_errorCode != WSA_IO_PENDING)
                        {
                            //Disconnect();
                            //这儿Disconnect()之后，不会再有OICP事件,所以OnDisconnected不会
                            //OnNetDisconnected();
                            return (-m_errorCode);
                        }
                    }
                    m_connectionState = connection_is_using;
                    return SUCCESS;
                }
                //CloseConnect();
                //OnNetDisconnected();
                //connection broken;
                
            }
            return (-m_errorCode);
        }
        else
        {
            m_connectionState = connection_is_using;
            return SUCCESS;
        }
    }
    else
    {
        return NOT_INITIALIZED;
    }
}

TInt32 CConnection::OnDisconnected()
{
    if (m_pAppCallBack)
    {
        return m_pAppCallBack->OnDissconneted(m_errorCode);
    }
    return SUCCESS;
}
//应用层发数据了
void CConnection::OnAppSend()
{
    if (m_appSeqNum == m_netConfirmNum)
    {
        ++ m_appSeqNum;
        TIOEvent event;
        event.m_seqNum              = m_appSeqNum;
        event.m_connectionIdx       = m_connectionIdx;
        event.m_connectionEvents    = event_connection_has_new_data_to_send;
        m_pEventQueues->AddAppEvent(event);
    }
}

//由应用层线程调用
void CConnection::OnAppDisconnected()
{
    m_appDisconnect = 1;
    if (m_appSeqNum == m_netConfirmNum)
    {
        ++ m_appSeqNum;
        TIOEvent event;
        event.m_seqNum              = m_appSeqNum;
        event.m_connectionIdx       = m_connectionIdx;
        event.m_connectionEvents    = event_connection_is_aborted;
        m_pEventQueues->AddAppEvent(event);
    }
}
//网络层发送了数据
void CConnection::OnNetSent()
{
    ++m_netConfirmNum;
}
//网络层收到了数据
void CConnection::OnNetRecv()
{
    if (m_netSeqNum == m_appConfirmNum)
    {
        ++m_netSeqNum;
        TIOEvent event;
        event.m_seqNum           = m_netSeqNum;
        event.m_connectionIdx    = m_connectionIdx;
        event.m_connectionEvents = event_connection_has_new_data_to_read;
        m_pEventQueues->AddNetEvent(event);
    }
}
//应用层收取了网络层的书
void CConnection::OnAppRecved()
{
    ++m_appConfirmNum;
}
//连接中断
void CConnection::OnNetDisconnected()
{
    m_netDisconnect = 1;
    if (m_netSeqNum == m_appConfirmNum)
    {
        ++m_netSeqNum;
        TIOEvent event;
        event.m_seqNum           = m_netSeqNum;
        event.m_connectionIdx    = m_connectionIdx;
        event.m_connectionEvents = event_connection_is_broken;
        m_pEventQueues->AddNetEvent(event);
    }
}

TInt32 CConnection::DisableNagle(const char chOpt)
{
   /* 
    * TCP_NODELAY	 BOOL=TRUE Disables the "nagle algorithm for send coalescing" which delays
    * short packets in the hope that the application will send more data and allow
    * it to combine them into a single one to improve network efficiency.
    */
    if (connection_is_using == m_connectionState)
    {
        TInt32 ret = setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
        if (ret == SOCKET_ERROR)
        {
            int errcode = WSAGetLastError();
            return -errcode;
        }
    }
    return CONNECTION_NOT_ESTABLISHED;
}

TInt32 CConnection::GetPendingDataLen()
{
    if (connection_is_using == m_connectionState)
    {
        return m_inPipe.GetDataLen();
    }
    return NOT_INITIALIZED;
}

}
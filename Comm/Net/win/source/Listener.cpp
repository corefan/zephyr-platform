#include "Listener.h"

namespace Zephyr
{

CListener::CListener()
{
    m_myIp = 0;
    m_listeningPort = 0;
    m_maxAcceptNr = 0;
    m_listeningSocket = SOCKET_ERROR;
    m_pAppData = NULL;
}
CListener::~CListener()
{
    if (SOCKET_ERROR != m_listeningSocket)
    {
        closesocket(m_listeningSocket);
    }
    
}
TInt32 CListener::Init(TUInt32 myIp,TUInt16 listeningPort,TUInt16 maxAcceptNr,void *pAppData)
{
    m_myIp = myIp;
    m_listeningPort = listeningPort;
    m_maxAcceptNr   = maxAcceptNr;
    m_pAppData      = pAppData;
    m_pNext         = NULL;
    
    m_listeningSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (SOCKET_ERROR == m_listeningSocket)
    {
        int errCode = GetLastError();

        return -errCode;
    }

    LINGER lingerStruct;
    lingerStruct.l_onoff = 1;
    lingerStruct.l_linger = 1;
    //use api's orignal types
    int ret = setsockopt(m_listeningSocket, SOL_SOCKET, SO_LINGER,
              (char *)&lingerStruct, sizeof(lingerStruct));
    if (ret == SOCKET_ERROR)
    {
        closesocket(m_listeningSocket);
        return FAIL;
    }
   /* 
    * TCP_NODELAY	 BOOL=TRUE Disables the "nagle algorithm for send coalescing" which delays
    * short packets in the hope that the application will send more data and allow
    * it to combine them into a single one to improve network efficiency.
    */
    const char chOpt = 1;
    ret = setsockopt(m_listeningSocket, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
    if (ret == SOCKET_ERROR)
    {
        closesocket(m_listeningSocket);
        return FAIL;
    }
    unsigned long argp   =   1;   
    ret   =   ioctlsocket(m_listeningSocket,FIONBIO,(unsigned   long*)&argp);
    if(ret   ==   SOCKET_ERROR)   
    {   
        closesocket(m_listeningSocket);
	    return   FAIL;
    }
    
    
    SOCKADDR_IN myAdd;
    SecureZeroMemory(&myAdd,sizeof(SOCKADDR_IN));
    myAdd.sin_port = htons(listeningPort);

	// Fill in the rest of the address structure
	myAdd.sin_family = AF_INET;
	myAdd.sin_addr.s_addr = myIp;

	// bind our name to the socket
	ret = bind(m_listeningSocket, 
		(LPSOCKADDR)&myAdd, 
		sizeof(struct sockaddr));
    if(ret   ==   SOCKET_ERROR)   
    {   
        closesocket(m_listeningSocket);   
        return FAIL;
    }
    ret = listen(m_listeningSocket, maxAcceptNr);
    
    if ( ret == SOCKET_ERROR )
	{
		closesocket(m_listeningSocket);
		return FALSE;
	}
	return SUCCESS;
    
}
    
SOCKET CListener::Accept()
{
    SOCKET acceptedSocket = SOCKET_ERROR;
    int nLen = sizeof(SOCKADDR_IN);
    acceptedSocket = WSAAccept(m_listeningSocket,
                               NULL,
                               &nLen,0,0); 
    return acceptedSocket;
}

TInt32 CListener::HasNewConnection()
{
    int nfds = 1;
    fd_set rset;
    FD_ZERO(&rset);

    struct timeval tm;
    tm.tv_sec=0;
    tm.tv_usec=0;

    FD_SET(m_listeningSocket,&rset);

    nfds = select(nfds, &rset,NULL,NULL,&tm);
    switch (nfds)
    {
        case 0:
        {
            return 0;
        }
        case -1:
        {
            return (-((TInt32)GetLastError()));
        }
        default:
        {
            return nfds;
        }
    }
}

}

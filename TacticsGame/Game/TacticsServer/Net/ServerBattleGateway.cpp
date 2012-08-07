#include "ServerBattleGateway.h"
#include "WindowsUtil.h"
#include "NetUtil.h"
#include "ServerMessageManager.h"

ServerBattleGateway* ServerBattleGateway::s_instance = NULL;

ServerBattleGateway* ServerBattleGateway::getInstance()
{
	if(s_instance)
		return s_instance;
	return s_instance = new ServerBattleGateway();
}

void ServerBattleGateway::destroyInstance()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance = NULL;
	}
}

ServerBattleGateway::ServerBattleGateway()
	: BattleGateway(), m_hListenSocket(INVALID_SOCKET), m_hAcceptEvent(WSA_INVALID_EVENT), 
	m_hShutdownEvent(NULL), m_hAcceptThread(INVALID_HANDLE_VALUE)
{

}

ServerBattleGateway::~ServerBattleGateway()
{
	deinit();
}

ReturnCode ServerBattleGateway::init()
{
	//Make sure we don't try to initialize twice
	if(m_bIsInitialized)
		return RC_ERR_INVALID_STATE;

	//Initialize WSA
	WSAData objWsaData;
	if(NO_ERROR != WSAStartup(MAKEWORD(2,2), &objWsaData))
		return RC_ERR_GENERAL;

	//Get the number of threads to create for each IOCP, based on the number of available processors
	int iNumProcessors = 1;
	WindowsUtil::GetNumProcessors(iNumProcessors);
	//int iNumThreads = SERVER_GATEWAY_NUM_THREADS_PER_PROCESSOR * iNumProcessors;

	//TODO: Make me more than one thread.
	int iNumThreads = 1;

	//Initialize IOCP's
	RET_IF_FAILED(NetUtil::CreateIocp(m_hIncomingPort, &ServerBattleGateway::IOCPIncomingThreadStub, iNumThreads));
	RET_IF_FAILED(NetUtil::CreateIocp(m_hOutgoingPort, &ServerBattleGateway::IOCPOutgoingThreadStub, iNumThreads));

	//TODO: Figure out a better mechanism here than hard-coded port/address
	//Create our listener socket
	RET_IF_FAILED(NetUtil::CreateBoundOverlappedSocket(m_hListenSocket,
		SERVER_GATEWAY_TEMP_LISTEN_PORT,
		SERVER_GATEWAY_TEMP_LISTEN_ADDRESS));

	//Start listener thread listening
	if(SOCKET_ERROR == listen(m_hListenSocket, SOMAXCONN))
	{
		deinit();
		return RC_ERR_GENERAL;
	}

	//Create an even which we'll use to listen for incoming connections over our listener socket
	if(WSA_INVALID_EVENT == (m_hAcceptEvent = WSACreateEvent()))
	{
		deinit();
		return RC_ERR_GENERAL;
	}

	//Bind our event to listen for incoming connections
	if(SOCKET_ERROR == WSAEventSelect(m_hListenSocket, m_hAcceptEvent, FD_ACCEPT))
	{
		deinit();
		return RC_ERR_GENERAL;
	}

	//Create an event to notify the gateway and all its threads that they need to shut down
	m_hShutdownEvent = CreateEvent(0, TRUE, FALSE, NULL);

	//Spin a new thread to handle acceptance events
	m_hAcceptThread = CreateThread(NULL, 0, &AcceptanceThreadStub, NULL, 0, 0);

	//Successfully initialized, yay!
	m_bIsInitialized = true;
	return RC_OK;
}

void ServerBattleGateway::deinit()
{
	//Set shutdown event
	SetEvent(m_hShutdownEvent);

	//Wait for shutdown to propagate, then close shutdown event
	WaitForSingleObject(m_hAcceptThread, INFINITE);
	WindowsUtil::CloseHandleNull(m_hShutdownEvent);

	//Close up our listener socket
	NetUtil::CloseSocket(m_hListenSocket);

	//Close our incoming connection event
	NetUtil::CloseWsaEvent(m_hAcceptEvent);

	//Clean up IO Completion Ports
	NetUtil::CloseIocp(m_hIncomingPort);
	NetUtil::CloseIocp(m_hOutgoingPort);

	//Clean up WSA
	WSACleanup();

	//Mark as deinitialized
	m_bIsInitialized = false;
}

DWORD WINAPI ServerBattleGateway::IOCPIncomingThreadStub(LPVOID lpParam)
{
	return ServerBattleGateway::getInstance()->IOCPIncomingThread(lpParam);
}

/**
 *	This method handles incoming parcels through the server's IOCP.  It is tasked with accommodating the following
 *	scenarios:
 *	
 *	<ul>
 *	<li>One parcel is sent in one packet</li>
 *	<li>One parcel is sent across multiple packets</li>
 *	<li>One packet contains pieces of multiple packet</li>
 *	<li>The received packet does not contain enough bytes to determine the size of the parcel</li>
 *	</ul>
 *	
 *	The <pre>IOCPIncomingThread</pre> method handles the first two scenarios by 1) determining the parcel size, 2)
 *	allocating enough memory to store the whole parcel to a 'parcel buffer', 3) pushing all received bytes into the 
 *	parcel buffer until it is full.  The parcel is then completed and sent off to the <pre>MessageManager</pre>.
 *	
 *	When the end of one parcel (or a whole parcel) and the beginning of another parcel is received in the same packet,
 *	<pre>IOCPIncomingThread</pre> pushes the remaining bytes of the first parcel to the parcel buffer as from step 3, 
 *	above.  It then moves the left over bytes in the packet to the front of the packet, and reprocesses it as from step
 *	1, above (as if it were an entirely new packet).
 *	
 *	The last scenario happens infrequently, but can occur especially as a result of how we handle the third scenario.
 *	Here, we use a small 'size buffer' (4 bytes) to contain whatever portion of the packet size we have received.  We
 *	continue receiving packets (generally just one) until we've received enough bytes to determine the size of the parcel
 *	being received.  We then prepend the completed parcel size bytes to the rest of the packet and continue on from step 1,
 *	above.
 *	
 *	TODO: Currently the project's error handling isn't well developed, and it is quite apparent in this method.  Special attention
 *	should be paid to updating this function once logging and other error handling methods are introduced.  Additionally, a lot of 
 *	work can be done to improve the robustness of this function to account for corrupt or malformed packets, but for now, handling
 *	the above scenarios is sufficient.
 */
DWORD ServerBattleGateway::IOCPIncomingThread(LPVOID lpParam)
{
	//Declare some necessary variables
	DWORD dwNumBytesTransferred = 0;
	OVERLAPPED* pOverlapped = NULL;
	BattleConnection* pConnection = NULL;
	DWORD dwBytesReceived = 0;
	DWORD dwError = 0;
	DWORD dwFlags = 0;

	//As long as we're not trying to shut down the gateway...
	while(WAIT_OBJECT_0 != WaitForSingleObject(m_hShutdownEvent, 0))
	{
		//Wait for IOCP to trigger thread
		if(!GetQueuedCompletionStatus(m_hIncomingPort, &dwNumBytesTransferred, (PULONG_PTR)&pConnection, &pOverlapped, INFINITE))
		{
			//TODO: Improve error handling here instead of just dropping the thread.
			break;
		}

		//TODO: Remove client from list of active clients when connection dropped
		//Check for invalid params or disconnected client
		if(!pConnection || dwNumBytesTransferred == 0)
			break;

		//Check if we're receiving a brand new parcel
		if(!pConnection->isTransferInProgress())
		{
			//Check bytes transfered to make sure we can pull size (ie, at least 4 bytes)
			if(dwNumBytesTransferred + pConnection->getBytesInSizeBuffer() >= sizeof(typeParcelSize))
			{
				//Grab any bytes stored in the size buffer
				typeParcelSize tpsSize;
				pConnection->pullParcelSizeFromBuffers(tpsSize);

				//If we've pulled enough, go ahead and allocate memory for the parcel, mark as transfer started
				pConnection->allocateParcelBuffer(tpsSize);

				//Push any content in the size buffer to the parcel buffer and note that these bytes have been 'transferred'
				pConnection->appendSizeBufftoParcelBuff();
				pConnection->addBytesTransferred(pConnection->getBytesInSizeBuffer());

				//Transfer has begun, yay!
				pConnection->setTransferInProgress(true);
			}else
			{
				//If we haven't pulled enough bytes to describe the parcel size, push all pulled bytes to the size buffer to access later.
				//Note that we know dwNumBytesTransferred + getBytesInSizeBuffer() is less than the size buffer, so this memcpy is fine
				memcpy(pConnection->getSizeBuffer() + pConnection->getBytesInSizeBuffer(), pConnection->getWsaBuffer()->buf, dwNumBytesTransferred);

				//Take note of total bytes pushed to size buffer
				pConnection->setBytesInSizeBuffer(dwNumBytesTransferred + pConnection->getBytesInSizeBuffer());

				//Continue to receive packages from this connection so we can complete the current parcel.
				int dwBytesSent = WSARecv(pConnection->getSocket(), pConnection->getWsaBuffer(), 1, &dwBytesReceived, &dwFlags, pConnection->getOverlapped(), NULL);
				if((SOCKET_ERROR == dwBytesSent) && (WSA_IO_PENDING != (dwError = WSAGetLastError())))
				{
					//TODO: Improve error handling here instead of just dropping the thread.
					break;
				}

				//Repeat loop so thread can continue to process incoming packets.
				continue;
			}
		}

		//Push as much of the rest of the parcel to the parcel buffer as we can.  Make sure not to push any of the next incoming parcel to the buffer.
		DWORD dwBytesToAppend = (dwNumBytesTransferred <= pConnection->getBytesLeftInParcel()) ? dwNumBytesTransferred : pConnection->getBytesLeftInParcel();
		if(RC_FAILED(pConnection->appendWsaBuffToParcelBuff(dwBytesToAppend)))
		{
			//Note that this shouldn't really ever happen with the checking we have above, but we should always handle something that can return 
			// an RC_ERR code.
			//TODO: Improve error handling here instead of just dropping the thread.
			break;
		}

		//Update total bytes pushed to the parcel buffer so far
		pConnection->addBytesTransferred(dwBytesToAppend);

		//Check to see if we've completed the current parcel
		if(pConnection->getBytesTransferred() == pConnection->getCurrentParcelSize())
		{
			//Convert raw data to parcel
			//TODO: Catch run-time errors here
			Parcel objParcel;
			if(RC_FAILED(BattleGateway::retrieveParcelFromBuffer(pConnection->getParcelBuffer(), pConnection->getCurrentParcelSize(), objParcel)))
			{
				//TODO: Yeah... this situation is also pretty bad... figure out a robust way of handling this issue.
				//TODO: Improve error handling here instead of just dropping the thread.
				break;
			}

			//Send parcel to the message manager for unpacking/execution
			ServerMessageManager::getInstance()->receiveParcel(objParcel);

			//Hold over any bytes from the next package so they can be processed as part of the next incoming parcel
			pConnection->moveRemainingBytesToFrontOfBuffer(dwNumBytesTransferred, dwBytesToAppend);

			//Reset for next transmission
			pConnection->clearConnection();

			//If bytes have been held over, process them as if from a new Recv() call.
			if(dwNumBytesTransferred > dwBytesToAppend)
			{
				PostQueuedCompletionStatus(m_hIncomingPort, dwNumBytesTransferred - dwBytesToAppend, (ULONG_PTR) pConnection, pConnection->getOverlapped());
				continue;
			}
		}else if(pConnection->getBytesTransferred() > pConnection->getCurrentParcelSize())
		{
			//TODO: Improve error handling here instead of just dropping the thread.
			break;
		}

		//Receive the message
		int dwBytesSent = WSARecv(pConnection->getSocket(), pConnection->getWsaBuffer(), 1, &dwBytesReceived, &dwFlags, pConnection->getOverlapped(), NULL);
		if((SOCKET_ERROR == dwBytesSent) && (WSA_IO_PENDING != (dwError = WSAGetLastError())))
		{
			//TODO: Handle error properly
			//RemoveConnection(pConnection);
			continue;
		}
	}

	return 0;
}

DWORD WINAPI ServerBattleGateway::IOCPOutgoingThreadStub(LPVOID lpParam)
{
	return ServerBattleGateway::getInstance()->IOCPOutgoingThread(lpParam);
}

DWORD ServerBattleGateway::IOCPOutgoingThread(LPVOID lpParam)
{
	//Declare some necessary variables
	DWORD dwNumBytesTransferred = 0;
	BattleConnection* pobjConnection = NULL;

	//Create a clean slate for the message we'll send
	char cMessage[256];
	ZeroMemory(cMessage, sizeof(cMessage));

	//As long as we're not trying to shut down the gateway...
	while(WAIT_OBJECT_0 != WaitForSingleObject(m_hShutdownEvent, 0))
	{
		//Wait for IOCP to trigger thread
		if(!GetQueuedCompletionStatus(m_hOutgoingPort, &dwNumBytesTransferred, (PULONG_PTR)&pobjConnection, (LPOVERLAPPED*)&cMessage, INFINITE))
			continue;

		//Check for valid parameters
		if(NULL == pobjConnection || NULL == cMessage)
			continue;

		//TODO: Main WSASend logic
	}

	return 0;
}

DWORD WINAPI ServerBattleGateway::AcceptanceThreadStub(LPVOID lpParam)
{
	return ServerBattleGateway::getInstance()->AcceptanceThread(lpParam);
}

DWORD ServerBattleGateway::AcceptanceThread(LPVOID lpvoid)
{
	//Declare necessary objects.
	WSANETWORKEVENTS objEvents;

	//As long as we're not trying to shut down the gateway...
	while(WAIT_OBJECT_0 != WaitForSingleObject(m_hShutdownEvent, 0))
	{
		if(WSA_WAIT_TIMEOUT != WSAWaitForMultipleEvents(1, &m_hAcceptEvent, FALSE, SERVER_GATEWAY_ACCEPT_THREAD_WAIT, FALSE))
		{
			WSAEnumNetworkEvents(m_hListenSocket, m_hAcceptEvent, &objEvents);
			if((FD_ACCEPT & objEvents.lNetworkEvents) && (0 == objEvents.iErrorCode[FD_ACCEPT_BIT]))
			{
				AcceptConnection();
			}
		}
	}

	//Shutdown time
	return 0;
}

ReturnCode ServerBattleGateway::AcceptConnection()
{
	//Create an address structure to contain info about the incoming connection
	sockaddr_in objIncomingAddress;
	int iAddressSize = sizeof(objIncomingAddress);

	//Accept the incoming connection
	SOCKET objIncomingSocket = accept(m_hListenSocket, (sockaddr*)&objIncomingAddress, &iAddressSize);
	if(INVALID_SOCKET == objIncomingSocket)
	{
		//TODO: Handle error here
		return RC_ERR_GENERAL;
	}

	//Create a ConnectionInformation object with relevant information about the connection established.
	BattleConnection* pobjConnectionInfo = new BattleConnection();
	pobjConnectionInfo->setSocket(objIncomingSocket);
	RET_RC_IF_FAILED(NetUtil::AssociateSocketWithIocp(m_hIncomingPort, objIncomingSocket, (void*)pobjConnectionInfo), RC_ERR_GENERAL);

	//Add connection to list of current connections
	AddConnection(pobjConnectionInfo);

	//Initialize data for the recv call
	DWORD dwBytesReceived = 0;
	DWORD dwError = 0;
	DWORD dwFlags = 0;
	WSABUF *p_wbuf = pobjConnectionInfo->getWsaBuffer();
	OVERLAPPED *p_ol = pobjConnectionInfo->getOverlapped();

	//Make an initial receive call, incoming thread will pick up when receive is finished.
	int dwBytesSent = WSARecv(objIncomingSocket, p_wbuf, 1, &dwBytesReceived, &dwFlags, p_ol, NULL);
	if((SOCKET_ERROR == dwBytesSent) && (WSA_IO_PENDING != (dwError = WSAGetLastError())))
	{
		return RC_ERR_GENERAL;
	}

	return RC_OK;
}

//NOTE: We assume that the message will always be a 256-byte character array.
//TODO: Implement this function with actual Parcel objects
ReturnCode ServerBattleGateway::broadcastParcel(Parcel &objParcel)
{
	return RC_ERR_NOT_IMPLEMENTED;

	/*
	CS.enter();
	typeConnectionVecIterator iter;
	for(iter = m_pobjConnectionVec.begin(); iter != m_pobjConnectionVec.end(); ++iter)
	{
		PostQueuedCompletionStatus(m_hOutgoingPort, 0, (ULONG_PTR)*iter, (LPOVERLAPPED)cMessage);
	}
	CS.leave();

	return RC_OK;
	*/
}
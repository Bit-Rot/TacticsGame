#include "NetUtil.h"

ReturnCode NetUtil::CreateSocket(SOCKET &hSocket)
{
	//Try to create a socket with some default values
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == hSocket)
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode NetUtil::CreateBoundSocket(SOCKET &hSocket, int iPort, const std::string &sAddress)
{
	//Create the socket
	RET_IF_FAILED(CreateSocket(hSocket));

	//Bind the socket
	if(RC_FAILED(BindSocket(hSocket, iPort, sAddress)))
	{
		closesocket(hSocket);
		return RC_ERR_GENERAL;
	}

	//Success!
	return RC_OK;
}

ReturnCode NetUtil::BindSocket(SOCKET &hSocket, int iPort, const std::string &sAddress)
{
	//Create our socket address structure
	sockaddr_in saAddress;
	saAddress.sin_family = AF_INET;
	saAddress.sin_port = htons(iPort);

	//Bind to either given IPv4 address or any address, depending on input
	if(!sAddress.empty())
		saAddress.sin_addr.S_un.S_addr = inet_addr(sAddress.c_str());
	else
		saAddress.sin_addr.s_addr = INADDR_ANY;

	//Bind the socket
	if(SOCKET_ERROR == bind(hSocket, (struct sockaddr*)&saAddress, sizeof(saAddress)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode NetUtil::CreateOverlappedSocket(SOCKET &hSocket)
{
	//Create the overlapped socket
	hSocket =  WSASocket(AF_INET, SOCK_STREAM, 0, 0, NULL, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == hSocket)
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}
	
ReturnCode NetUtil::CreateBoundOverlappedSocket(SOCKET &hSocket, int iPort, const std::string &sAddress)
{
	//Try and create the overlapped socket.
	RET_IF_FAILED(CreateOverlappedSocket(hSocket));

	//Bind the socket
	if(RC_FAILED(BindSocket(hSocket, iPort, sAddress)))
	{
		closesocket(hSocket);
		return RC_ERR_GENERAL;
	}

	//Success!
	return RC_OK;
}


ReturnCode NetUtil::AssociateSocketWithIocp(HANDLE &hIocp, SOCKET &hSocket, void* pParam)
{
	if(NULL == CreateIoCompletionPort((HANDLE)hSocket, hIocp, (ULONG_PTR)pParam, 0))
	{
		return RC_ERR_GENERAL;
	}

	return RC_OK;
}

ReturnCode NetUtil::CreateIocp(HANDLE &hPort, LPTHREAD_START_ROUTINE pWortkerThread, int iNumThreads)
{
	//Create the port
	hPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if(NULL == hPort)
		return RC_ERR_GENERAL;

	//Determine the number of threads to spawn and spawn them
	for(int i=0; i<iNumThreads; ++i)
	{
		CreateThread(NULL, 0, pWortkerThread, NULL, 0, NULL);
	}

	//Success!
	return RC_OK;
}

ReturnCode NetUtil::ConnectSocket(SOCKET &hSocket, std::string sIpAddress, int iPortNumber)
{
	//Create our address object
	sockaddr_in objSockAddress;
	objSockAddress.sin_addr.S_un.S_addr = inet_addr(sIpAddress.c_str());
	objSockAddress.sin_port = htons(iPortNumber);
	objSockAddress.sin_family = AF_INET;

	//Note the size of the address object
	int iAddressSize = sizeof(sockaddr_in);

	//Connect
	if(SOCKET_ERROR == connect(hSocket, (sockaddr*)&objSockAddress, iAddressSize))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}
#ifndef NETUTIL_H
#define NETUTIL_H

#include <winsock2.h>
#include <windows.h>
#include <string>
#include "TacticsErrors.h"

class NetUtil
{
public:
	/**
	 *	@brief Creates a TCP data stream socket.  Note that any socket created
	 *			with the use of this function must be closed using closesocket().
	 *	@param [OUT] hSocket - the socket created upon the success of this function.
	 *	@return RC_OK if successful.  RC_ERR_GENERAL if we failed to create the socket.
	 */
	static ReturnCode CreateSocket(SOCKET &hSocket);

	/**
	 *	@brief Creates a TCP data stream socket bound to the given port and address.
	 *			Note that any socket successfully created with the use of this function
	 *			must be closed using closesocket().
	 *	@param [OUT] hSocket - the socket created upon the success of this function
	 *	@param [IN] iPort - the port to which the created socket is to be bound
	 *	@param [IN] sAddress - a string representation of the IPv4 address to which the socket
	 *						   is to be bound.  If sAddress is empty, CreateBoundSocket assumes
	 *						   the socket should be bound to any available address (INADDR_ANY)
	 *	@return RC_OK if successful.  RC_ERR_GENERAL if we failed to create or bind the socket.
	 */
	static ReturnCode CreateBoundSocket(SOCKET &hSocket, int iPort, const std::string &sAddress);

	/**
	 *	@brief Binds a TCP data stream socket to the given port and address.
	 *	@param [OUT] hSocket - the socket which is to be bound
	 *	@param [IN] iPort - the port to which the created socket is to be bound
	 *	@param [IN] sAddress - a string representation of the IPv4 address to which the socket
	 *						   is to be bound.  If sAddress is empty, BindSocket assumes
	 *						   the socket should be bound to any available address (INADDR_ANY)
	 *	@return RC_OK if successful.  RC_ERR_GENERAL if we failed to create or bind the socket.
	 */
	static ReturnCode BindSocket(SOCKET &hSocket, int iPort, const std::string &sAddress);
	
	/**
	 *	@brief Creates an overlapped TCP data stream socket.  Note that any socket created
	 *			with the use of this function must be closed using closesocket().
	 *	@param [OUT] hSocket - the socket created upon the success of this function.
	 *	@return RC_OK if successful.  RC_ERR_GENERAL if we failed to create the socket.
	 */
	static ReturnCode CreateOverlappedSocket(SOCKET &hSocket);
	
	/**
	 *	@brief Creates an overlapped TCP data stream socket bound to the given port and address.
	 *			Note that any socket successfully created with the use of this function must be 
	 *			closed using closesocket().
	 *	@param [OUT] hSocket - the socket created upon the success of this function
	 *	@param [IN] iPort - the port to which the created socket is to be bound
	 *	@param [IN] sAddress - a string representation of the IPv4 address to which the socket
	 *						   is to be bound.  If sAddress is empty, CreateBoundSocket assumes
	 *						   the socket should be bound to any available address (INADDR_ANY)
	 *	@return RC_OK if successful.  RC_ERR_GENERAL if we failed to create or bind the socket.
	 */
	static ReturnCode CreateBoundOverlappedSocket(SOCKET &hSocket, int iPort, const std::string &sAddress);

	/**
	 *	@brief Associates the given socket with the given IO Completion Port.
	 *	@param [IN] hIocp - the IOCP to associate the given socket with.
	 *	@param [IN] hSocket - the socket to associate with the given IOCP.
	 *	@param [IN] pParam - a pointer to a parameter object to be passed in through the "CompletionKey" parameter
	 *						 of GetQueuedCompletionStatus.  Is NULL if not specified.
	 *	@return RC_OK if successful.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode AssociateSocketWithIocp(HANDLE &hIocp, SOCKET &hSocket, void* pParam = NULL);

	/**
	 *	@brief Creates an IOCP and spawns a number of threads which are to be associated with it.
	 *	@param [OUT] hPort - a handle to the IOCP which will be created
	 *	@param [IN] pWorkerThread - a pointer to the thread routine which will be spawned a number of times
	 *								as a result of calling this function
	 *	@param [IN] iNumThreads - the number of threads to spawn
	 *	@return RC_OK if successful.  RC_ERR_GENERAL if we failed to create the IOCP.
	 */
	static ReturnCode CreateIocp(HANDLE &hPort, LPTHREAD_START_ROUTINE pWortkerThread = NULL, int iNumThreads = 0);

	/**
	 *	@brief Connects a given socket to the given IP address and Port number.
	 *	@param [INOUT] hSocket - the socket to connect
	 *	@param [IN] sIpAddress - the IP address to connect to
	 *	@param [IN] iPortNumber - the port number to connect to
	 *	@return RC_OK if successful.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode ConnectSocket(SOCKET &hSocket, std::string sIpAddress, int iPortNumber);

	/**
	 *	@brief Closes the given socket and assigns it the value INVALID_SOCKET
	 *	@param [INOUT] hSocket - the socket to close
	 *	@return Always returns RC_OK;
	 */
	static inline ReturnCode CloseSocket(SOCKET &hSocket)
	{
		if(INVALID_SOCKET != hSocket)
		{
			closesocket(hSocket);
			hSocket = INVALID_SOCKET;
		}
		return RC_OK;
	}
	
	/**
	 *	@brief Closes the given WSA event and assigns it the value WSA_INVALID_EVENT
	 *	@param [INOUT] hEvent - the WSA event to close
	 *	@return Always returns RC_OK;
	 */
	static inline ReturnCode CloseWsaEvent(HANDLE &hEvent)
	{
		if(WSA_INVALID_EVENT != hEvent)
		{
			WSACloseEvent(hEvent);
			hEvent = WSA_INVALID_EVENT;
		}
		return RC_OK;	
	}
	
	/**
	 *	@brief Closes the given event and assigns it the value INVALID_EVENT
	 *	@param [INOUT] hEvent - the socket to close
	 *	@return Always returns RC_OK;
	 */
	static inline ReturnCode CloseEvent(HANDLE &hEvent)
	{
		if(INVALID_HANDLE_VALUE != hEvent)
		{
			::CloseHandle(hEvent);
			hEvent = INVALID_HANDLE_VALUE;
		}
		return RC_OK;
	}

	/**
	 *	@brief Closes the given IOCP and assigns it the value NULL
	 *	@param [INOUT] hPort - the socket to close
	 *	@return Always returns RC_OK;
	 */
	static inline ReturnCode CloseIocp(HANDLE &hPort)
	{
		if(NULL != hPort)
		{
			::CloseHandle(hPort);
			hPort = NULL;
		}
		return RC_OK;
	}

protected:

private:

};


#endif //#ifndef NETUTIL_H
#ifndef SERVERBATTLEGATEWAY_H
#define SERVERBATTLEGATEWAY_H

#include "BattleGateway.h"

#define SERVER_GATEWAY_NUM_THREADS_PER_PROCESSOR 2
#define SERVER_GATEWAY_TEMP_LISTEN_PORT 5150
#define SERVER_GATEWAY_TEMP_LISTEN_ADDRESS "" //blank string will result in INADDR_ANY in util code
#define SERVER_GATEWAY_ACCEPT_THREAD_WAIT 100

class ServerBattleGateway : public BattleGateway
{
public:
	//Singleton stuffs
	static ServerBattleGateway* getInstance();
	static void destroyInstance();
	virtual ReturnCode init();
	virtual void deinit();
	
	/**
	 *	@brief Send the given parcel over the network to a specific client
	 *	
	 *	@param [IN] objParcel - the parcel to send to the server
	 */
	ReturnCode broadcastParcel(Parcel &objParcel);

	//TODO: Implement this method and add parameters to allow distinction of correct client to send to.
	/**
	 *	@brief Send the given parcel over the network to a specific client
	 *	
	 *	@param [IN] objParcel - the parcel to send to the server
	 */
	//ReturnCode sendParcel(const Parcel &objParcel);

protected:
	//Owned Properties
	SOCKET m_hListenSocket;
	WSAEVENT m_hAcceptEvent;
	HANDLE m_hShutdownEvent;
	HANDLE m_hAcceptThread;

	//Protected methods
	DWORD IOCPIncomingThread(LPVOID lpParam);
	static DWORD WINAPI IOCPIncomingThreadStub(LPVOID lpParam);
	DWORD IOCPOutgoingThread(LPVOID lpParam);
	static DWORD WINAPI IOCPOutgoingThreadStub(LPVOID lpParam);
	DWORD AcceptanceThread(LPVOID lpParam);
	static DWORD WINAPI AcceptanceThreadStub(LPVOID lpParam);
	ReturnCode AcceptConnection();

private:
	//Singleton stuffs
	ServerBattleGateway();
	~ServerBattleGateway();
	static ServerBattleGateway* s_instance;
};

#endif//#ifndef SERVERBATTLEGATEWAY_H
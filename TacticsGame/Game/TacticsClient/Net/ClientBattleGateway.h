#ifndef CLIENTBATTLEGATEWAY_H
#define CLIENTBATTLEGATEWAY_H

//Work only on the local host for the time being.
#define CLIENTGATEWAY_SERVER_IP_ADDRESS "127.0.0.1"
#define CLIENTGATEWAY_SERVER_PORT_NUMBER 5150

#include "BattleGateway.h"

class ClientBattleGateway : public BattleGateway
{
public:
	//Singleton stuffs
	static ClientBattleGateway* getInstance();
	static void destroyInstance();

	//Public methods

	/**
	 *	Initialize WSA and connect to the server.
	 *	@Return.  RC_OK if successful.  RC_ERR_GENERAL if WSA failed to initialize or we failed to connect to the server.
	 */
	virtual ReturnCode init();

	/**
	 *	Close the <pre>ClientBattleGateway</pre>.  Clean up sockets, WSA, and various data members.
	 */
	virtual void deinit();

	/**
	 *	@brief Send the given parcel over the network to the server.
	 *	
	 *	@param [IN] objParcel - the parcel to send to the server
	 */
	ReturnCode sendParcel(const Parcel &objParcel);

protected:
	SOCKET m_hServerSocket;
private:
	//Singleton stuffs
	ClientBattleGateway();
	~ClientBattleGateway();
	static ClientBattleGateway* s_instance;
};

#endif //#ifndef CLIENTBATTLEGATEWAY_H
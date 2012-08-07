#include "ClientBattleGateway.h"
#include "NetUtil.h"


ClientBattleGateway* ClientBattleGateway::s_instance = NULL;

ClientBattleGateway* ClientBattleGateway::getInstance()
{
	if(s_instance)
		return s_instance;
	return s_instance = new ClientBattleGateway();
}

void ClientBattleGateway::destroyInstance()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance = NULL;
	}
}

ClientBattleGateway::ClientBattleGateway()
	: BattleGateway(), m_hServerSocket(INVALID_SOCKET)
{

}

ClientBattleGateway::~ClientBattleGateway()
{
	deinit();
}

ReturnCode ClientBattleGateway::init()
{
	//Check if we've already initialized
	if(m_bIsInitialized)
		return RC_ERR_INVALID_STATE;

	//Initialize WSA
	WSAData objWsaData;
	if(NO_ERROR != WSAStartup(MAKEWORD(2,2), &objWsaData))
		return RC_ERR_GENERAL;

	//Create our server socket and connect to server
	RET_RC_IF_FAILED(NetUtil::CreateSocket(m_hServerSocket), RC_ERR_GENERAL);
	RET_RC_IF_FAILED(NetUtil::ConnectSocket(m_hServerSocket, CLIENTGATEWAY_SERVER_IP_ADDRESS, CLIENTGATEWAY_SERVER_PORT_NUMBER), RC_ERR_GENERAL);

	//Success!
	m_bIsInitialized = true;
	return RC_OK;
}

void ClientBattleGateway::deinit()
{
	//Close the server socket
	NetUtil::CloseSocket(m_hServerSocket);

	//Clean up WSA
	WSACleanup();

	//Note deinitialized status
	m_bIsInitialized = false;
}

ReturnCode ClientBattleGateway::sendParcel(const Parcel &objParcel)
{
	//Make sure we're initialized
	if(!m_bIsInitialized)
		return RC_ERR_INVALID_STATE;

	//Convert the parcel to a parcel buffer
	char* pcParcelBuffer = NULL;
	RET_RC_IF_FAILED(BattleGateway::createParcelBuffer(objParcel, pcParcelBuffer), RC_ERR_GENERAL);

	//Send the message to the server
	ReturnCode rc = RC_OK;
	if(SOCKET_ERROR == send(m_hServerSocket, pcParcelBuffer, objParcel.size, 0))
		rc = RC_ERR_GENERAL;

	//Free memory allocated to buffer
	delete [] pcParcelBuffer;
	pcParcelBuffer = NULL;

	return rc;
}
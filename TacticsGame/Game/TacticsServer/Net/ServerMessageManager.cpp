#include "ServerMessagemanager.h"
#include "BattleGateway.h"
#include "ServerBattleGateway.h"

ServerMessageManager* ServerMessageManager::s_instance = NULL;

ServerMessageManager* ServerMessageManager::getInstance()
{
	if(s_instance)
		return s_instance;
	return s_instance = new ServerMessageManager();
}

void ServerMessageManager::destroyInstance()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance = NULL;
	}
}

ServerMessageManager::ServerMessageManager()
{

}

ServerMessageManager::~ServerMessageManager()
{

}

ReturnCode ServerMessageManager::sendMessage(Message &objMessage)
{
	//Create and zero out a parcel
	Parcel objParcel;
	RET_RC_IF_FAILED(packMessageIntoParcel(objMessage, objParcel), RC_ERR_GENERAL);

	//Send the parcel through the client gateway
	ReturnCode rc = RC_OK;
	if(RC_FAILED(ServerBattleGateway::getInstance()->broadcastParcel(objParcel)))
		rc = RC_ERR_GENERAL;

	return rc;
}
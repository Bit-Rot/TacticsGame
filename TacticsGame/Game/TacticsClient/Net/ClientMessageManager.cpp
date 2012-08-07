#include "ClientMessageManager.h"
#include "ClientBattleGateway.h"


ClientMessageManager* ClientMessageManager::s_instance = NULL;

ClientMessageManager* ClientMessageManager::getInstance()
{
	if(s_instance)
		return s_instance;
	return s_instance = new ClientMessageManager();
}

void ClientMessageManager::destroyInstance()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance = NULL;
	}
}

ClientMessageManager::ClientMessageManager()
{

}

ClientMessageManager::~ClientMessageManager()
{

}

ReturnCode ClientMessageManager::sendMessage(Message &objMessage)
{
	//Pack message into a parcel
	Parcel objParcel;
	RET_RC_IF_FAILED(packMessageIntoParcel(objMessage, objParcel), RC_ERR_GENERAL);

	//Send the parcel through the client gateway
	ReturnCode rc = RC_OK;
	if(RC_FAILED(ClientBattleGateway::getInstance()->sendParcel(objParcel)))
		rc = RC_ERR_GENERAL;

	//Destroy the parcel to avoid memory leaks and return
	BattleGateway::destroyParcel(objParcel);
	return rc;
}
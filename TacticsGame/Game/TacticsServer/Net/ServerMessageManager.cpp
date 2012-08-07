#include "ServerMessagemanager.h"
#include "BattleGateway.h"

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

//TODO: Finish @return comment
ReturnCode ServerMessageManager::sendMessage(Message &objMessage)
{
	//Create and zero out a parcel
	Parcel objParcel;
	ZeroMemory(&objParcel, sizeof(objParcel));

	//Fill in the parcel with message information
	RET_RC_IF_FAILED(objMessage.getMessageBuffer(&(objParcel.message_buff), objParcel.message_size), RC_ERR_GENERAL);
	objParcel.message_type = objMessage.getClassId();

	//Determine the size of the parcel
	objParcel.size = sizeof(objParcel.size) + sizeof(objParcel.message_type) + sizeof(objParcel.message_size) + objParcel.message_size;

	//send message via the appropriate gateway
	char* pcParcelBuffer = NULL;
	BattleGateway::createParcelBuffer(objParcel, pcParcelBuffer);


	return RC_ERR_NOT_IMPLEMENTED;
}
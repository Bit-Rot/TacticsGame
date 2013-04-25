#include "MessageManager.h"
#include "BattleGateway.h"
#include <Windows.h>

ReturnCode MessageManager::receiveParcel(Parcel &objParcel)
{
	//Unpack parcel as a message
	Message* pobjMessage = NULL;
	RET_RC_IF_FAILED(extractMessageFromParcel(objParcel, pobjMessage), RC_ERR_INVALID_STATE);

	//Execute message
	ReturnCode rc = RC_OK;
	if(RC_FAILED(pobjMessage->execute()))
		rc = RC_ERR_GENERAL;

	//Delete the message and destroy the parcel
	delete pobjMessage;
	BattleGateway::destroyParcel(objParcel);

	//Cool beans
	return rc;
}

ReturnCode MessageManager::extractMessageFromParcel(const Parcel &objparcel, Message *& pobjMessage)
{
	//Allocate a new message of the appropriate type
	RET_RC_IF_FAILED(Message::allocateMessageById((MessageClassId::MessageClassId)objparcel.message_type, pobjMessage), RC_ERR_GENERAL);

	//Initialize the message
	RET_RC_IF_FAILED(pobjMessage->init(objparcel.message_buff, objparcel.message_size), RC_ERR_INVALID_STATE);

	//Looks like we're in the clear
	return RC_OK;
}

ReturnCode MessageManager::packMessageIntoParcel(const Message &objMessage, Parcel &objParcel)
{
	//Clear output
	ZeroMemory(&objParcel, sizeof(objParcel));

	//Fill in the parcel with message information
	RET_RC_IF_FAILED(objMessage.getMessageBuffer(&(objParcel.message_buff), objParcel.message_size), RC_ERR_GENERAL);
	objParcel.message_type = objMessage.getClassId();

	//Determine the size of the parcel
	objParcel.size = sizeof(objParcel.size) + sizeof(objParcel.message_type) + sizeof(objParcel.message_size) + objParcel.message_size;

	//Return successfully
	return RC_OK;
}

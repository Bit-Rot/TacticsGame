#include "Message.h"

#include "PingMessage.h"
#include "ChangeEntityPositionMessage.h"
#include "AddEntityMessage.h"

Message::Message()
	: m_bIsWellFormed(false)
{

}

DWORD Message::getBaseMessageSize() const
{
	//Right now the base message class has no members to be packed into a message buffer, so it is size 0.
	return 0;
}


ReturnCode Message::allocateMessageById(MessageClassId::MessageClassId enumMessageId, Message *& pobjMessage)
{
	//Translate message id to a message object
	switch(enumMessageId)
	{
	case MessageClassId::AddEntityMessageClassId:
		pobjMessage = new AddEntityMessage();
		break;
	case MessageClassId::ChangeEntityPositionMessageClassId:
		pobjMessage = new ChangeEntityPositionMessage();
		break;
	case MessageClassId::PingMessageClassId:
		pobjMessage = new PingMessage();
		break;
	default:
		//Message ID doesn't correspond to a known message type
		return RC_ERR_INVALID_ARGS;
	}

	//Good to go!
	return RC_OK;
}


/*
ReturnCode Message::initBaseMessage(char* pcBuffer, DWORD dwBufferSize)
{
	//No base properties to initialize yet.  Return success.
	return RC_OK;
}
*/
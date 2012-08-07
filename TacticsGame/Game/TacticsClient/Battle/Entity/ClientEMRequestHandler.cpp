#include "ClientEMRequestHandler.h"
#include "EntityManager.h"
#include "ClientMessageManager.h"
#include "PingMessage.h"
#include <windows.h>

//TODO: Remove me.  I should only be able to talk to the messenger, not the gateway
//#include "ClientBattleGateway.h"


ClientEMRequestHandler::ClientEMRequestHandler()
	: EMRequestHandler(), m_bIsOnline(false)
{

}

ClientEMRequestHandler::~ClientEMRequestHandler()
{

}

ReturnCode ClientEMRequestHandler::requestAddEntity(Entity* pobjEntity)
{
	//If not connected to a server, we'll just go ahead and cut to the chase.
	if(!m_bIsOnline)
		return EntityManager::getInstance()->addEntity(pobjEntity);

	//Otherwise, we'll need to notify the server that we want to create a new entity

	//Create a new Message

	//Package the entity

	//Sent the message to the messenger

	return RC_ERR_NOT_IMPLEMENTED;
}

ReturnCode ClientEMRequestHandler::requestRemoveEntity(Entity* pobjEntity)
{
	//If not connected to a server, we'll just go ahead and cut to the chase.
	if(!m_bIsOnline)
		return EntityManager::getInstance()->removeEntity(pobjEntity);

	//TODO: Implement message sending to client here

	return RC_ERR_NOT_IMPLEMENTED;
}

ReturnCode ClientEMRequestHandler::requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY)
{
	//TODO: Remove this heinous monstrosity once unit tests are implemented
	/*
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//Create the parcel
	Parcel objParcel;
	PingMessage objMessage;
	objMessage.getMessageBuffer(&(objParcel.message_buff), objParcel.message_size);
	objParcel.message_type = objMessage.getClassId();

	objParcel.size = sizeof(objParcel.size) + sizeof(objParcel.message_type) + sizeof(objParcel.message_size) + objParcel.message_size;

	char* pcParcelBuffer;
	BattleGateway::createParcelBuffer(objParcel, pcParcelBuffer);

	//TEST SENDING TWO PARCELS IN ONE BUFFER
	char* pcDoubleBuffer = new char[objParcel.size*2];
	memcpy(pcDoubleBuffer, pcParcelBuffer, objParcel.size);
	memcpy(pcDoubleBuffer+objParcel.size, pcParcelBuffer, objParcel.size);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//ClientBattleGateway::getInstance()->sendMessage(pcParcelBuffer, objParcel.size);

	//TEST: Completely inundate server with messages to weed out a particular issue.
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);
	ClientBattleGateway::getInstance()->sendParcel(pcDoubleBuffer, objParcel.size*2);

	//TODO: Deallocate these in a better place
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	delete[] objParcel.message_buff;
	delete[] pcParcelBuffer;
	delete[] pcDoubleBuffer;
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	*/
	PingMessage objMessage;
	ClientMessageManager::getInstance()->sendMessage(objMessage);

	//If not connected to a server, we'll just go ahead and cut to the chase.
	if(!m_bIsOnline)
		return EntityManager::getInstance()->changeEntityPosition(pobjEntity, iX, iY);

	//TODO: Implement message sending to client here

	return RC_OK;
}

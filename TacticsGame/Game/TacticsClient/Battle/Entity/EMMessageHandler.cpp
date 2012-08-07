#include "EMMessageHandler.h"
#include "EntityManager.h"

EMMessageHandler::EMMessageHandler()
{
}

EMMessageHandler::~EMMessageHandler()
{
}

ReturnCode EMMessageHandler::requestAddEntity(Entity* pobjEntity)
{
	return EntityManager::getInstance()->addEntity(pobjEntity);
}

ReturnCode EMMessageHandler::requestRemoveEntity(Entity* pobjEntity)
{
	return EntityManager::getInstance()->removeEntity(pobjEntity);
}

ReturnCode EMMessageHandler::requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY)
{
	return EntityManager::getInstance()->changeEntityPosition(pobjEntity, iX, iY);
}


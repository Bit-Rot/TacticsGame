#include "ServerEMRequestHandler.h"
#include "EntityManager.h"
#include <windows.h>

ServerEMRequestHandler::ServerEMRequestHandler()
	: EMRequestHandler()
{

}

ServerEMRequestHandler::~ServerEMRequestHandler()
{

}

ReturnCode ServerEMRequestHandler::requestAddEntity(Entity* pobjEntity)
{
	return RC_ERR_NOT_IMPLEMENTED;
}

ReturnCode ServerEMRequestHandler::requestRemoveEntity(Entity* pobjEntity)
{
	return RC_ERR_NOT_IMPLEMENTED;
}

ReturnCode ServerEMRequestHandler::requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY)
{
	return RC_ERR_NOT_IMPLEMENTED;
}

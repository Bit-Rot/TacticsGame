#include "EntityManager.h"
#include <windows.h>

EntityManager* EntityManager::m_instance = NULL;

EntityManager::EntityManager()
	: m_pobjLandscape(NULL), m_pobjRequestHandler(NULL), m_pobjMessageHandler(NULL)
{}

EntityManager::~EntityManager()
{
	CS.enter();

	//Destroy all entities
	typeEntityIterator iter;
	for(iter = m_pobjEntitySet.begin(); iter != m_pobjEntitySet.end(); iter++)
	{
		delete (*iter);
	}
	m_pobjEntitySet.clear();

	//Destroy request and message handlers simultaneously if they're the same
	if(m_pobjMessageHandler == m_pobjRequestHandler)
	{
		delete m_pobjRequestHandler;
		m_pobjRequestHandler = NULL;
		m_pobjMessageHandler = NULL;
	}

	//Destroy our request handler
	if(m_pobjRequestHandler)
	{
		delete m_pobjRequestHandler;
		m_pobjRequestHandler = NULL;
	}

	//Destroy our message handler
	if(m_pobjMessageHandler)
	{
		delete m_pobjMessageHandler;
		m_pobjMessageHandler = NULL;
	}
	CS.leave();
}

EntityManager* EntityManager::getInstance()
{
	if(m_instance)
		return m_instance;
	return m_instance = new EntityManager();
}

void EntityManager::destroyInstance()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void EntityManager::update(float dt)
{
	//Update each contained entity, in no particular order
	typeEntityIterator iter;
	CS.enter();
	for(iter = m_pobjEntitySet.begin(); iter != m_pobjEntitySet.end(); iter++)
	{
		//Update the current entity
		(*iter)->update(dt);
	}
	CS.leave();
}

ReturnCode EntityManager::requestAddEntity(Entity* pobjEntity)
{
	//Make sure our request handler is in place
	if(!m_pobjRequestHandler) 
		return RC_ERR_INVALID_STATE;

	//Let the RequestHandler handle the request
	return m_pobjRequestHandler->requestAddEntity(pobjEntity);
}

ReturnCode EntityManager::requestRemoveEntity(Entity* pobjEntity)
{
	//Make sure our request handler is in place
	if(!m_pobjRequestHandler) 
		return RC_ERR_INVALID_STATE;

	//Let the RequestHandler handle the request
	return m_pobjRequestHandler->requestRemoveEntity(pobjEntity);
}

ReturnCode EntityManager::requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY)
{
	//Make sure our request handler is in place
	if(!m_pobjRequestHandler) 
		return RC_ERR_INVALID_STATE;

	//Let the RequestHandler handle the request
	return m_pobjRequestHandler->requestChangeEntityPosition(pobjEntity, iX, iY);
}


//TODO: Update with client/server implementation in mind
ReturnCode EntityManager::addEntity(Entity* pobjEntity)
{
	CS.enter();
	m_pobjEntitySet.insert(pobjEntity);
	CS.leave();
	return RC_OK;
}

//TODO: Update with client/server implementation in mind
ReturnCode EntityManager::removeEntity(Entity* pobjEntity)
{
	CS.enter();
	m_pobjEntitySet.erase(pobjEntity);
	CS.leave();
	return RC_OK;
}

//TODO: Update with client/server implementation in mind
ReturnCode EntityManager::changeEntityPosition(Entity* pobjEntity, int iX, int iY)
{
	//Check for valid input
	if(!pobjEntity)
		return RC_ERR_INVALID_ARGS;

	//Check for invalid state
	if(!m_pobjLandscape)
		return RC_ERR_INVALID_STATE;

	//Get desired GridUnit from landscape
	GridUnit* pobjDesiredGridUnit;
	if(RC_FAILED(m_pobjLandscape->getGridUnit(iX, iY, pobjDesiredGridUnit)))
		return RC_ERR_GENERAL;

	//Check if GridUnit is currently occupied
	if(pobjDesiredGridUnit->getEntity())
		return RC_ERR_GENERAL;

	//Remove entity from previous grid unit
	GridUnit* pobjPreviousGridUnit = pobjEntity->getGridUnit();
	if(!pobjPreviousGridUnit || RC_FAILED(pobjPreviousGridUnit->removeEntity()))
		return RC_ERR_GENERAL;

	//Add given entity to grid unit
	if(RC_FAILED(pobjDesiredGridUnit->addEntity(pobjEntity)))
		return RC_ERR_GENERAL;
	pobjEntity->setGridUnit(pobjDesiredGridUnit);

	//Success!
	return RC_OK;
}

ReturnCode EntityManager::getEntityById(UINT uiEntityId, Entity *& pobjEntity)
{
	//Iterate through all entities
	typeEntityIterator objIter;
	for(objIter = m_pobjEntitySet.begin(); objIter != m_pobjEntitySet.end(); objIter++)
	{
		if(uiEntityId == (*objIter)->getId())
		{
			//Found it!
			pobjEntity = *objIter;
			return RC_OK;
		}
	}

	//Didn't find it D:
	return RC_ERR_GENERAL;
}


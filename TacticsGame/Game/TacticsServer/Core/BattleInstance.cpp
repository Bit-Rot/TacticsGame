#include "BattleInstance.h"
#include <windows.h>
#include "LandscapeGenerator.h"
#include "EntityManager.h"
#include "ServerBattleGateway.h"
#include "ServerEMRequestHandler.h"

//To handle user input
#include <iostream>

int main(int argc, char* argv[])
{
	BattleInstance::getInstance()->run();
	return 0;
}

BattleInstance* BattleInstance::m_instance = NULL;

BattleInstance* BattleInstance::getInstance()
{
	if(m_instance)
		return m_instance;
	return m_instance = new BattleInstance();
}

void BattleInstance::destroyInstance()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

BattleInstance::BattleInstance()
	: m_bIsRunning(false), m_pobjLandscape(NULL)
{

}

BattleInstance::~BattleInstance()
{
	//Destroy landscape and its grid units
	LandscapeGenerator::getInstance()->DestroyLandscape(m_pobjLandscape);
	LandscapeGenerator::destroyInstance();

	//TODO: Move this when we no longer call init() in BattleGameState
	ServerBattleGateway::destroyInstance();

	//Destroy Singletons
	EntityManager::destroyInstance();
	LandscapeGenerator::destroyInstance();
}

ReturnCode BattleInstance::run()
{
	//Make sure we don't abuse public access of run() method somehow
	if(m_bIsRunning)
		return RC_ERR_INVALID_STATE;

	init();

	//Wait for user input to break
	char a;
	while(1)
	{
		std::cin >> a;
		if(a == 'q')
			break;
		Sleep(1000);
	}

	deinit();

	//Note that we've stopped running
	m_bIsRunning = false;
	return RC_OK;
}

ReturnCode BattleInstance::init()
{
	//Generate our landscape randomly
	RET_RC_IF_FAILED(LandscapeGenerator::getInstance()->GenerateTestLandscape(m_pobjLandscape), RC_ERR_GENERAL);
	EntityManager::getInstance()->setLandscape(m_pobjLandscape);

	//Request & Message handlers are the same for the server
	ServerEMRequestHandler* pobjRequestHandler = new ServerEMRequestHandler();
	EntityManager::getInstance()->setRequestHandler(pobjRequestHandler);
	EntityManager::getInstance()->setMessageHandler(pobjRequestHandler);

	//Initialize the gateway
	ServerBattleGateway::getInstance()->init();

	return RC_OK;
}

ReturnCode BattleInstance::deinit()
{
	LandscapeGenerator::getInstance()->DestroyLandscape(m_pobjLandscape);
	LandscapeGenerator::destroyInstance();
	EntityManager::destroyInstance();
	ServerBattleGateway::getInstance()->deinit();
	ServerBattleGateway::destroyInstance();

	return RC_OK;
}
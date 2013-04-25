#include "BattleGameState.h"
#include "BattleViewManager.h"
#include "Landscape.h"
#include "LandscapeGenerator.h"
#include "EntityManager.h"
#include "ClientEMRequestHandler.h"
#include "ClientBattleGateway.h"
#include "EMMessageHandler.h"

//Note, we don't use this class directly in this file, we just need to destroy any remaining instances to prevent memory leaks
#include "ClientMessageManager.h"

//TODO: Remove Me
#include "PudEntity/PudEntity.h"
#include "PudEntity/PudEntityView.h"

BattleGameState::BattleGameState()
	: GameState(), m_pobjLandscape(NULL)
{}

BattleGameState::~BattleGameState()
{
	//Undo init call
	deinit();

	//Destroy singletons
	LandscapeGenerator::destroyInstance();
	BattleViewManager::destroyInstance();
	EntityManager::destroyInstance();

	//NOTE: This is here only to prevent memory leaks, though we don't use the message manager directly in this class
	ClientMessageManager::destroyInstance();
}

ReturnCode BattleGameState::init(const TacticsGame &objGame)
{
	//Create our ViewManager
	ID3D10Device* pobjDevice = objGame.getDevice();
	RET_RC_IF_FAILED(BattleViewManager::getInstance()->init(pobjDevice, m_objCamera), RC_ERR_GENERAL);
	BattleViewManager::getInstance()->calculateWvpMatrix(objGame.getClientWidth(), objGame.getClientHeight());

	//Generate our landscape randomly
	RET_RC_IF_FAILED(LandscapeGenerator::getInstance()->GenerateTestLandscape(m_pobjLandscape), RC_ERR_GENERAL);

	//Create Landscape View
	LandscapeView* pobjLandscapeView = new LandscapeView(m_pobjLandscape);
	RET_RC_IF_FAILED(LandscapeView::buildViewForTestLandscape(*m_pobjLandscape, *pobjLandscapeView), RC_ERR_GENERAL);
	
	//Set vital data members of singleton classes
	BattleViewManager::getInstance()->setLandscapeView(pobjLandscapeView);
	EntityManager::getInstance()->setLandscape(m_pobjLandscape);
	ClientEMRequestHandler* pobjRequestHandler = new ClientEMRequestHandler();
	EntityManager::getInstance()->setRequestHandler(pobjRequestHandler);
	EMMessageHandler* pobjMessageHandler = new EMMessageHandler();
	EntityManager::getInstance()->setMessageHandler(pobjMessageHandler);
	
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TEST CODE HERE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//Push a PudEntity to the grid
	PudEntity* pobjPudEntity = new PudEntity();
	PudEntityController* pobjPudEntityController = new PudEntityController();
	pobjPudEntity->setController((EntityController*&)pobjPudEntityController);
	GridUnit* pobjGridUnit = NULL;
	RET_RC_IF_FAILED(m_pobjLandscape->getGridUnit(0, 0, pobjGridUnit), RC_ERR_GENERAL);
	pobjPudEntity->setGridUnit(pobjGridUnit);
	RET_RC_IF_FAILED(pobjGridUnit->addEntity((Entity*)pobjPudEntity), RC_ERR_GENERAL);
	
	//Create PudEntityView
	PudEntityView* pobjPudEntityView = new PudEntityView();
	PudEntityView::buildViewForPudEntity(*pobjPudEntityView);
	pobjPudEntityView->setEntity((Entity*)pobjPudEntity);

	//Add PudEntity and its PudEntityView to managers
	RET_RC_IF_FAILED(EntityManager::getInstance()->requestAddEntity(pobjPudEntity), RC_ERR_GENERAL);
	RET_RC_IF_FAILED(BattleViewManager::getInstance()->addEntityView((EntityView*)pobjPudEntityView), RC_ERR_GENERAL);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//TODO: Move this to somewhere better.  Network activity should not begin before BattleGameState initialization.
	ClientBattleGateway::getInstance()->init();

	//Return success/failure status
	return RC_OK;
}

void BattleGameState::deinit()
{
	//Destroy landscape and its grid units
	LandscapeGenerator::getInstance()->DestroyLandscape(m_pobjLandscape);
	LandscapeGenerator::destroyInstance();

	//TODO: Move this when we no longer call init() in BattleGameState
	ClientBattleGateway::destroyInstance();
}

void BattleGameState::resize(int iClientWidth, int iClientHeight)
{
	BattleViewManager::getInstance()->onResize(iClientWidth, iClientHeight);
}

void BattleGameState::update(float dt)
{
	//Update the camera
	m_objCamera.update(dt);

	//Update the entities
	EntityManager::getInstance()->update(dt);
}

void BattleGameState::draw()
{
	//Note that we can return error codes when drawing.  I don't think we need to worry about it at this level,
	// we should simply trace any errors as they occur so they can be gracefully noted/debugged later.
	BattleViewManager::getInstance()->draw();
}
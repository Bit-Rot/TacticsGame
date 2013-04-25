//=============================================================================
// Init Direct3D.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//=============================================================================

#include "TacticsGame.h"
#include "BattleGameState.h"
#include "DirectUtils.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	TacticsGame theApp(hInstance);

	theApp.initApp();

	return theApp.run();
}

TacticsGame::TacticsGame(HINSTANCE hInstance)
	: D3DApp(hInstance), m_pobjCurrentState(NULL)
{
}

TacticsGame::~TacticsGame()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	if(m_pobjCurrentState)
	{
		delete m_pobjCurrentState;
		m_pobjCurrentState = NULL;
	}
}

void TacticsGame::initApp()
{
	D3DApp::initApp();

	//Initialize our BattleState
	m_pobjCurrentState = new BattleGameState();
	m_pobjCurrentState->init(*this);

	//Initialize Bidness!
}

void TacticsGame::onResize()
{
	D3DApp::onResize();

	if(m_pobjCurrentState)
		m_pobjCurrentState->resize(m_iClientWidth, m_iClientHeight);
	
	//Resize Bidness!
}

void TacticsGame::updateScene(float dt)
{
	D3DApp::updateScene(dt);

	m_pobjCurrentState->update(dt);
	//Update game logic.
}

void TacticsGame::drawScene()
{
	D3DApp::drawScene();
	//Reset stuff here because drawing the text sets it to stuff we don't like.

	m_pobjCurrentState->draw();

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);

	mSwapChain->Present(0, 0);
}
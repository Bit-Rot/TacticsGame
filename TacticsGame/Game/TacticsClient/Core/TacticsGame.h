#ifndef TACTICSGAME_H
#define TACTICSGAME_H

#include "d3dApp.h"
#include "GameState.h"

class TacticsGame : public D3DApp
{
public:
	TacticsGame(HINSTANCE hInstance);
	~TacticsGame();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene();

	ID3D10Device* getDevice() const {return md3dDevice;}

protected:

	GameState *m_pobjCurrentState;
};

#endif //#ifndef TACTICSGAME_H
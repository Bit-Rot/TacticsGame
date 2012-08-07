#ifndef BATTLEGAMESTATE_H
#define BATTLEGAMESTATE_H

#include "TacticsGame.h"
#include "GameState.h"
#include "Landscape.h"
#include "LandscapeView.h"
#include "LandscapeController.h"
#include "Camera.h"

class BattleGameState : public GameState
{
public:
	BattleGameState();
	virtual ~BattleGameState();

	virtual ReturnCode init(const TacticsGame &objGame);
	virtual void deinit();
	virtual void resize(int iClientWidth, int iClientHeight);
	virtual void update(float dt);
	virtual void draw();

	Landscape* getLandscape(){return m_pobjLandscape;}
protected:
	Landscape* m_pobjLandscape;
	Camera m_objCamera;

private:
};

#endif //#ifndef BATTLEGAMESTATE_H
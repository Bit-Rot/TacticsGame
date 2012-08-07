#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ViewManager.h"
#include "TacticsErrors.h"

class TacticsGame;

class GameState
{
public:
	virtual ~GameState(){}

	virtual ReturnCode init(const TacticsGame &objGame) = 0;
	virtual void deinit() = 0;
	virtual void resize(int iClientWidth, int iClientHeight) = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;

protected:

private:

};

#endif //ifndef GAMESTATE_H
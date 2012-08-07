#ifndef BATTLEINSTANCE_H
#define BATTLEINSTANCE_H

#include "TacticsErrors.h"
#include "Landscape.h"

class BattleInstance
{
public:
	//Singleton stuffs
	static BattleInstance* getInstance();
	static void destroyInstance();

	ReturnCode run();

protected:
	//Owned members
	bool m_bIsRunning;
	Landscape* m_pobjLandscape;

private:
	//Singleton stuffs
	BattleInstance();
	~BattleInstance();
	static BattleInstance* m_instance;

	ReturnCode init();
	ReturnCode deinit();
};

#endif //#ifndef BATTLEINSTANCE_H
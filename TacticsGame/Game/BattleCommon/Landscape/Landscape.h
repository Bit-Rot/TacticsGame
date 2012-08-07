#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "GridUnit.h"
#include "LandscapeController.h"
#include "TacticsErrors.h"
#include "SyncUtil.h"

class BattleGameState;

/**
*	The <pre>Landscape</pre> is the grid/map on which units are placed.  It contains a 
*	multi-dimensional array of <pre>GridUnit<pre>s and some basic business logic.
*/
class Landscape
{
public:
	Landscape(int iGridWidth, int iGridLength, LandscapeController* &pobjLandscapeController);
	Landscape(int iGridWidth, int iGridLength);
	~Landscape();

	//Setters
	ReturnCode setGridUnit(int iGridWidth, int iGridLength, GridUnit* gridUnit);
	void setLandscapeController(LandscapeController* &pobjLandscapeController){m_pobjLandscapeController = pobjLandscapeController;}

	//Getters
	ReturnCode getGridUnit(int iGridWidth, int iGridLength, GridUnit* &gridUnit);
	int getGridWidth() const {return m_iGridWidth;}
	int getGridLength() const {return m_iGridLength;}
	LandscapeController* getLandscapeController() const {return m_pobjLandscapeController;}

protected:
	//Aggregate properties
	LandscapeController *m_pobjLandscapeController;

	//Owned properties
	int m_iGridWidth;
	int m_iGridLength;
	GridUnit ***grid;
	CriticalSectionEx CS;
private:

};

#endif //#ifndef LANDSCAPE_H
#ifndef GRIDUNIT_H
#define GRIDUNIT_H

#include "TacticsErrors.h"

class Entity;

enum GridUnitTerrain
{
	GrassTerrain,
	SandTerrain,
	WaterTerrain,
	LavaTerrain
};

/**
 *	The <pre>GridUnit</pre> class represents a single tile on the landscape's grid.  Currently,
 *	the grid only accommodates one grid unit per tile, though we may modify this in the future
 *	such that each x-y coordinate can contain more than one grid unit (eg, think of a bridge or
 *	doorway).
 */
class GridUnit
{
public:
	GridUnit(int iBase, int iHeight, int iX, int iZ, GridUnitTerrain eTerrain);
	~GridUnit();

	ReturnCode addEntity(Entity* pobjEntity);
	ReturnCode removeEntity();

	//Getters
	int getBase() const {return m_iBase;}
	int getHeight() const {return m_iHeight;}
	int getX() const {return m_iX;}
	int getZ() const {return m_iZ;}
	GridUnitTerrain getTerrain() const {return m_eTerrain;}
	Entity* getEntity(){return m_pobjEntity;}

private:

protected:
	//Aggregate properties
	Entity* m_pobjEntity;

	//Owned properties
	//TODO: Introduce different heights for each vertex
	int m_iBase, m_iHeight, m_iX, m_iZ;
	GridUnitTerrain m_eTerrain;
};


#endif //#ifndef GRIDUNIT_H
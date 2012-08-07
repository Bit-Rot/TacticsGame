#include "Landscape.h"
#include "SyncUtil.h"
#include "EntityManager.h"
#include <Windows.h>

Landscape::Landscape(int iGridWidth, int iGridLength, LandscapeController* &pobjLandscapeController)
	: m_pobjLandscapeController(pobjLandscapeController)
{
	Landscape(iGridWidth, iGridLength);
}

Landscape::Landscape(int iGridWidth, int iGridLength)
	: grid(NULL), m_pobjLandscapeController(NULL)
{
	//Determine grid dimensions
	m_iGridWidth = iGridWidth;
	m_iGridLength = iGridLength;

	//Handle invalid grid width
	if(m_iGridWidth <= 0)
		m_iGridWidth = 1;

	//Handle invalid grid length
	if(m_iGridLength <= 0)
		m_iGridLength = 1;

	//Allocate grid
	CS.enter();
	grid = new GridUnit**[m_iGridWidth];
	for(int i=0; i<m_iGridWidth; ++i)
	{
		grid[i] = new GridUnit*[m_iGridLength];
		for(int j=0; j<m_iGridLength; ++j)
		{
			//just initialize grid locations to null... will instantiate later.
			grid[i][j] = NULL;
		}
	}
	CS.leave();
}

Landscape::~Landscape()
{
	//Destroy grid
	if(grid)
	{
		CS.enter();
		//Delete individual lengthwise-rows
		for(int i=0; i<m_iGridWidth; ++i)
		{
			if(grid[i])
			{
				delete[] grid[i];
				grid[i] = NULL;
			}
		}

		//Delete whole grid
		delete[] grid;
		grid = NULL;
		CS.leave();
	}

	//Destroy controller
	if(m_pobjLandscapeController)
	{
		CS.enter();
		delete m_pobjLandscapeController;
		m_pobjLandscapeController = NULL;
		CS.leave();
	}
}

ReturnCode Landscape::getGridUnit(int iGridWidth, int iGridLength, GridUnit *&objGridUnit)
{
	//Check boundaries and return grid unit point if all checks out
	objGridUnit = NULL;
	if(iGridWidth >= 0 && iGridWidth < m_iGridWidth && iGridLength >= 0 && iGridLength < m_iGridLength)
	{
		CS.enter();
		objGridUnit = grid[iGridWidth][iGridLength];
		CS.leave();
		return RC_OK;
	}

	//Out of bounds
	return RC_ERR_NOT_FOUND;
}

ReturnCode Landscape::setGridUnit(int iGridWidth, int iGridLength, GridUnit* objGridUnit)
{
	//Check boundaries and return grid unit point if all checks out
	if(iGridWidth >= 0 && iGridWidth < m_iGridWidth && iGridLength >= 0 && iGridLength < m_iGridLength)
	{
		CS.enter();
		grid[iGridWidth][iGridLength] = objGridUnit;
		CS.leave();
		return RC_OK;
	}

	//Out of bounds
	return RC_ERR_NOT_FOUND;
}
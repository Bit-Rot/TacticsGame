#include "GridUnit.h"
#include <windows.h>

GridUnit::GridUnit(int iBase, int iHeight, int iX, int iZ, GridUnitTerrain eTerrain)
	: m_iBase(iBase), m_iHeight(iHeight), m_iX(iX), m_iZ(iZ), m_eTerrain(eTerrain), m_pobjEntity(NULL)
{}

GridUnit::~GridUnit()
{}

ReturnCode GridUnit::addEntity(Entity* pobjEntity)
{
	//Check to make sure location isn't occupied already
	if(m_pobjEntity)
		return RC_ERR_INVALID_STATE;

	m_pobjEntity = pobjEntity;
	return RC_OK;
}

ReturnCode GridUnit::removeEntity()
{
	if(!m_pobjEntity)
		return RC_ERR_INVALID_STATE;

	m_pobjEntity = NULL;
	return RC_OK;
}
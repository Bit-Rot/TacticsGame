#include "PudEntity/PudEntity.h"
#include "EntityManager.h"
#include "MathUtil.h"

PudEntity::PudEntity()
	: m_fMovesPerSecond(10.0f), m_fSecondsSinceLastMove(0.0f)
{}

PudEntity::~PudEntity()
{}

//TODO: This code is really ugly.  We need to clean it up, refactor out code that can be refactored.
//		 The only reason this state is acceptable is because we'll need to go back and clean it up once
//		 we rework the EntityManager to communicate with a server anyway.
void PudEntity::update(float dt)
{
	//Update entity properties
	m_fSecondsSinceLastMove += dt;

	//Check for a valid controller to get instructions from
	if(m_pobjController)
	{
		//Gather desired x/z coordinate change
		int iDesiredX = getController()->getDesiredXDelta();
		int iDesiredZ = getController()->getDesiredZDelta();

		//Only continue if we've actually want to move the entity
		if(!iDesiredX && !iDesiredZ)
			return;

		//Try to move along the x-axis
		iDesiredX += m_pobjCurrentGridUnit->getX();
		iDesiredZ += m_pobjCurrentGridUnit->getZ();

		//Check to see if it's been long enough that we can move
		float fSecondsBetweenMoves = 0.0f;
		if(RC_SUCCESS(MathUtil::FrequencyToSeconds(m_fMovesPerSecond, fSecondsBetweenMoves))
			&& m_fSecondsSinceLastMove >= fSecondsBetweenMoves)
		{
			//Request position change
			if(RC_SUCCESS(EntityManager::getInstance()->requestChangeEntityPosition(this, iDesiredX, iDesiredZ)))
			{
				//TODO: Update once EntityManager checks with server for move success
				m_fSecondsSinceLastMove = 0.0f;
			}
		}
	}
}

PudEntityController* PudEntity::getController()
{
	return (PudEntityController*)m_pobjController;
}
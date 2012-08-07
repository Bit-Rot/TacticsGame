#include "Entity.h"
#include "EntityManager.h"
//#include <windows.h> <-- included by EntityManager

Entity::Entity()
	: m_pobjController(NULL), m_uiId(ENTITY_ID_UNSET), m_pobjCurrentGridUnit(NULL)
{}

Entity::~Entity()
{
	if(m_pobjController)
	{
		delete m_pobjController;
		m_pobjController = NULL;
	}
}
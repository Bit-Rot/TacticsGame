#ifndef ENTITY_H
#define ENTITY_H

#include "EntityController.h"
#include "GridUnit.h"
#include <windows.h>

#define ENTITY_ID_UNSET

/**
 *	The <pre>EntityClassId</pre> namespace is used simply as a wrapper to the <pre>EntityClassId</pre> 
 *	enum in order to facilitate clean syntax.
 */
namespace EntityClassId
{
	/**
	 * The <pre>EntityClassId</pre> enum is used to create a set of unique identifying numbers, one for 
	 * each subclass of the <pre>Entity</pre> class, such as to enable the serialization of <pre>Entity</pre>
	 * subclasses with integer values.
	 */
	enum EntityClassId
	{
		InvalidEntityClassId = -1,
		PudEntityClassId = 1
	};
}

/**
 *	An <pre>Entity</pre> is an interactive object which is bound to the <pre>Landscape</pre>.  
 *	Entities may be controlled by human or artificial intelligence, and can range anywhere
 *	from spell effects to units.
 */
class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void update(float dt) = 0;

	//Getters
	UINT getId() const {return m_uiId;}
	GridUnit* getGridUnit() const {return m_pobjCurrentGridUnit;}

	//Setters
	void setGridUnit(GridUnit* &pobjGridUnit){m_pobjCurrentGridUnit = pobjGridUnit;}
	void setId(UINT uiId){m_uiId = uiId;}
	void setController(EntityController* &pobjController){m_pobjController = pobjController;}

protected:
	EntityController* m_pobjController;
	UINT m_uiId;
	GridUnit* m_pobjCurrentGridUnit;

private:

};

struct Entity_Desc
{
public:
	UINT m_uiId;
};

#endif //#ifndef ENTITY_H
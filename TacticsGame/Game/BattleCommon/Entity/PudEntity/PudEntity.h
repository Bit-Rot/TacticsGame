#ifndef PUDENTITY_H
#define PUDENTITY_H

#include "Entity.h"
#include "PudEntity/PudEntityController.h"

/**
 *	A <pre>PudEntity</pre> is a basic test entity for use in the battle state.
 */
class PudEntity : public Entity
{
public:
	PudEntity();
	~PudEntity();

	virtual void update(float dt);

protected:
	float m_fMovesPerSecond, m_fSecondsSinceLastMove;

private:
	PudEntityController* getController();
};

struct PudEntity_Desc : public Entity_Desc
{

};

#endif //#ifndef PUDENTITY_H
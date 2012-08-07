#ifndef PUDENTITYCONTROLLER_H
#define PUDENTITYCONTROLLER_H

#include "EntityController.h"

/**
 *	<pre>PudEntityController</pre> is the controller component corresponding to the <pre>PudEntity</pre> class.
 */
class PudEntityController: public EntityController
{
public:
	//C'tor and D'tor
	PudEntityController();
	~PudEntityController();

	//Public methods
	int getDesiredXDelta() const;
	int getDesiredZDelta() const;

protected:

private:

};

#endif //#ifndef PUDENTITYCONTROLLER_H
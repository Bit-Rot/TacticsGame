#ifndef EMREQUESTHANDLER_H
#define EMREQUESTHANDLER_H

#include "TacticsErrors.h"
#include "Entity.h"

/**
 *	The <pre>EMRequestHandler</pre> serves as a template for both the client and server request handlers.
 *	The idea behind this class is to ensure as much common functionality between the way the client and 
 *	server interact with the state/model of the game, while allowing each to define it's own behavior
 *	accordingly.
 */
class EMRequestHandler
{
public:
	virtual ~EMRequestHandler();

	//Model-Modification Request Methods.
	virtual ReturnCode requestAddEntity(Entity* pobjEntity) = 0;
	virtual ReturnCode requestRemoveEntity(Entity* pobjEntity) = 0;
	virtual ReturnCode requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY) = 0;

protected:

private:

};


#endif EMREQUESTHANDLER_H
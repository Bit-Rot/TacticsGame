#ifndef EMMESSAGEHANDLER_H
#define EMMESSAGEHANDLER_H

#include "EMRequestHandler.h"

/**
 *	The <pre>EMMessageHandler</pre> provides an interface for interacting with the <pre>EntityManager</pre>.
 *	Specifically designed as an interface for messages on the client side, the 'request' methods here simply
 *	call directly on the corresponding <pre>EntityManager</pre> Model-Modification methods.  We provide this
 *	extra interface for the client in particular, because the <pre>ClientEMRequestHandler</pre> logic does not
 *	apply to handling message requests, whereas the <pre>ServerEMRequestHandler</pre> applies to both handling
 *	entity and message requests.
 */
class EMMessageHandler : public EMRequestHandler
{
public:
	//C'tor and D'tor
	EMMessageHandler();
	~EMMessageHandler();

	//Public methods
	virtual ReturnCode requestAddEntity(Entity* pobjEntity);
	virtual ReturnCode requestRemoveEntity(Entity* pobjEntity);
	virtual ReturnCode requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY);

protected:


private:

};

#endif //#ifndef EMMESSAGEHANDLER_H
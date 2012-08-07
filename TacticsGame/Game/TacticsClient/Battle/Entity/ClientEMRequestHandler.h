#ifndef CLIENTEMREQUESTHANDLER_H
#define CLIENTEMREQUESTHANDLER_H

#include "EMRequestHandler.h"

/**
 *	The <pre>ClientEMRequestHandler</pre> provides an interface for interacting with the <pre>EntityManager</pre>.
 *	If the client is running in standalone mode, it will forward requests directly to the <pre>EntityManager</pre>,
 *	and the desired action will be taken.  However, if running in online mode, the <pre>ClientEMRequestHandler</pre>
 *	will send a message to the server requesting that the given action be taken, after which the client will wait to 
 *	receive confirmation that an action is taken before updating the client's state.
 */
class ClientEMRequestHandler : public EMRequestHandler
{
public:
	//C'tor and D'tor
	ClientEMRequestHandler();
	~ClientEMRequestHandler();

	//Model-Modification Request Methods.
	virtual ReturnCode requestAddEntity(Entity* pobjEntity);
	virtual ReturnCode requestRemoveEntity(Entity* pobjEntity);
	virtual ReturnCode requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY);

protected:
	//TODO: Implement something more sophisticated than this garbage :D
	bool m_bIsOnline;

private:

};


#endif //#ifndef CLIENTEMREQUESTHANDLER_H
#ifndef SERVEREMREQUESTHANDLER_H
#define SERVEREMREQUESTHANDLER_H

#include "EMRequestHandler.h"
#include "Entity.h"
#include "TacticsErrors.h"

class ServerEMRequestHandler : public EMRequestHandler
{
public:
	//C'tor and D'tor
	ServerEMRequestHandler();
	~ServerEMRequestHandler();

	//Model-Modification Request Methods.
	virtual ReturnCode requestAddEntity(Entity* pobjEntity);
	virtual ReturnCode requestRemoveEntity(Entity* pobjEntity);
	virtual ReturnCode requestChangeEntityPosition(Entity* pobjEntity, int iX, int iY);

protected:

private:

};


#endif //#ifndef SERVEREMREQUESTHANDLER_H
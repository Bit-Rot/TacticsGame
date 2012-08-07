#ifndef SERVERMESSAGEMANAGER_H
#define SERVERMESSAGEMANAGER_H

#include "MessageManager.h"
#include "Parcel.h"
#include "TacticsErrors.h"
#include "Message.h"

class ServerMessageManager : public MessageManager
{
public:

	//Singleton stuffs
	static ServerMessageManager* getInstance();
	static void destroyInstance();

	//TODO: Add comment
	virtual ReturnCode sendMessage(Message &objMessage);
	
protected:

private:

	//Singleton stuffs
	ServerMessageManager();
	~ServerMessageManager();
	static ServerMessageManager* s_instance;
};

#endif //#ifndef SERVERMESSAGEMANAGER_H
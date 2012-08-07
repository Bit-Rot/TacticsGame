#ifndef CLIENTMESSAGEMANAGER_H
#define CLIENTMESSAGEMANAGER_H

#include "MessageManager.h"
#include "Parcel.h"
#include "TacticsErrors.h"
#include "Message.h"

class ClientMessageManager : public MessageManager
{
public:

	//Singleton stuffs
	static ClientMessageManager* getInstance();
	static void destroyInstance();

	//TODO: Add comment
	virtual ReturnCode sendMessage(Message &objMessage);

protected:

private:

	//Singleton stuffs
	ClientMessageManager();
	~ClientMessageManager();
	static ClientMessageManager* s_instance;
};

#endif //#ifndef CLIENTMESSAGEMANAGER_H
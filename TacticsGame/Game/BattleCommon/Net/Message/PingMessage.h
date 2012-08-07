#ifndef PINGMESSAGE_H
#define PINGMESSAGE_H

#include "Message.h"

class PingMessage : public Message
{
public:
	//C'tors and D'tors
	PingMessage();
	~PingMessage();

	//Public virtual methods
	virtual ReturnCode init(char* pcBuffer, DWORD dwBufferSize);
	virtual ReturnCode execute();
	virtual MessageClassId::MessageClassId getClassId() const;
	virtual ReturnCode getMessageBuffer(char** pcBuffer, DWORD &dwSize) const;

	//Getters and Setters
	int getSomeInt(){return m_iSomeInt;}
	DWORD getSomeStringSize(){return m_dwSomeStringSize;}
	char* getSomeString(){return m_pcSomeString;}

protected:
	//Protected virtual methods
	virtual DWORD getMessageSize() const;

	//Protected Properties
	int m_iSomeInt;
	DWORD m_dwSomeStringSize;
	char* m_pcSomeString;
private:

};


#endif //#ifndef PINGMESSAGE_H
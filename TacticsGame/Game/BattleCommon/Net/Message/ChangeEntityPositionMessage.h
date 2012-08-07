#ifndef CHANGEENTITYPOSITIONMESSAGE_H
#define CHANGEENTITYPOSITIONMESSAGE_H

#include "Message.h"

class ChangeEntityPositionMessage : public Message
{
public:
	//C'tor and D'tor
	ChangeEntityPositionMessage();
	~ChangeEntityPositionMessage();

	//Public virtual methods
	virtual ReturnCode init(char* pcBuffer, DWORD dwBufferSize);
	virtual ReturnCode execute();
	virtual MessageClassId::MessageClassId getClassId() const;
	virtual ReturnCode getMessageBuffer(char** pcBuffer, DWORD &dwSize) const;

protected:
	//Protected virtual methods
	virtual DWORD getMessageSize() const;


private:
	//Private members
	UINT m_uiEntityId;
	int m_iX, m_iY;

};
#endif //ifndef CHANGEENTITYPOSITIONMESSAGE_H
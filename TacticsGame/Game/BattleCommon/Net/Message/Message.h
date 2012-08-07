#ifndef MESSAGE_H
#define MESSAGE_H

#include "TacticsErrors.h"
#include <Windows.h>

/**
 *	A namespace wrapper around the MessageClassId for convenience.
 */
namespace MessageClassId
{
	/**
	 *	Note that whenever a new MessageClassId enum is added, the <pre>allocateMessageById</pre> method
	 *	must be updated or it will not function properly.
	 */
	enum MessageClassId
	{
		InvalidMessageClassId = -1,
		PingMessageClassId = 1,
		ChangeEntityPositionMessageClassId,
		AddEntityMessageClassId
	};
};

class Message
{
public:
	Message();

	//Public virtual methods 
	virtual ReturnCode init(char* pcBuffer, DWORD dwBufferSize) = 0;
	virtual ReturnCode execute() = 0;
	virtual MessageClassId::MessageClassId getClassId() const = 0;

	/**
	 *	@brief Create and return a message buffer describing this message
	 *	
	 *	@param [OUT] pcBuffer - a pointer to the buffer which will be created
	 *	@param [OUT] dwSize - the size of the buffer
	 *	@return RC_OK if successful.
	 */
	virtual ReturnCode getMessageBuffer(char** pcBuffer, DWORD &dwSize) const = 0;

	//Static methods
	
	/**
	 *	@brief Get a pointer to a new message object of the type associated with the given MessageClassId.
	 *		   Note that this method allocates memory to the returned <pre>Message</pre> pointer, and that
	 *		   this will need to be deallocated by the caller.
	 *	
	 *	@param [IN] enumMessageId - the MessageClassId corresponding to the desired message.
	 *	@param [OUT] pobjMessage - a pointer to the allocated message
	 *	@return RC_OK if successful, RC_ERR_INVALID_ARGS if enumMessageId does not match a known message type.
	 */
	static ReturnCode allocateMessageById(MessageClassId::MessageClassId enumMessageId, Message *& pobjMessage);
protected:
	//Protected virtual methods
	virtual DWORD getMessageSize() const = 0;

	//Protected methods
	DWORD getBaseMessageSize() const;
	//ReturnCode initBaseMessage(char* pcBuffer, DWORD dwBufferSize);

	//Protected properties
	//Set to true during construction if message was constructed successfully, otherwise false.
	bool m_bIsWellFormed;
private:
};


#endif //#ifndef MESSAGE_H
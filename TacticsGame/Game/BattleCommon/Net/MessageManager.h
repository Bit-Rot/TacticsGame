#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Parcel.h"
#include "TacticsErrors.h"
#include "Message.h"

/**
 *	The <pre>MessageManager</pre> is responsible for the packing and unpacking of messages and parcels, respectively,
 *	and facilitating the communication between client/server gateways and local components.  The <pre>MessageManager</pre>
 *	itself fulfills only the components of these tasks which are not specific to the client or server architecture, and
 *	is correspondingly subclassed by the client and server to provide more specific functionality.
 */
class MessageManager
{
public:

	//Public methods
	 
	//NOTE: We may need to introduce a message queue and some form of concurrent message processing
	//		along with some other fancy functionality at some point in the future.  For now, network
	//		communication is simple enough that this is likely entirely unnecessary.
	/**
	 *	@brief Unpack the message contained within the parcel, initialize it, and execute it.
	 *	
	 *	@param [IN] objParcel - the parcel object to unpack
	 *	@return RC_OK if successful.  RC_ERR_INVALID_STATE if the parcel fails to extract, otherwise
	 *			an ReturnCode corresponding to the failure of the message execution.
	 */
	ReturnCode receiveParcel(Parcel &objParcel);

	/**
	 *	@brief Pack the message into a parcel and send it to the gateway to be shipped.
	 *	
	 *	@param [IN] objMessage - the message to be packed and sent
	 *	@return RC_OK if successful.  RC_ERR_GENERAL otherwise.
	 */
	virtual ReturnCode sendMessage(Message &objMessage) = 0;

protected:

	//Private methods
	/**
	 *	@brief Extract the message object from the given Parcel.
	 *	
	 *	@param [IN] objParcel the parcel from which the message will be extracted
	 *	@param [OUT] pobjMessage a pointer to the extracted message
	 *	@return RC_OK if successful, RC_ERR_GENERAL if the parcel contains an unknown message type,
	 *	        RC_ERR_INVALID_STATE if there was an error extracting the message from the parcel.
	 */
	ReturnCode extractMessageFromParcel(const Parcel &objParcel, Message *& pobjMessage);

	/**
	 *	@brief Pack message into a parcel.  Will allocate memory to the parcel's message buffer, which
	 *	will need to be deallocated, or will cause memory leaks.
	 *	
	 *	NOTE: Currently it is not known if there are any cases in which we will need to deallocate memory
	 *		  assigned to the parcel's message_buff variable if <pre>packMessageIntoParcel</pre> fails. This
	 *		  description may need modification once more message subclasses have been implemented.
	 *	
	 *	@param [IN] objMessage the message to be packed into a parcel
	 *	@param [OUT] objParcel the packed parcel.
	 *	@return RC_OK if successful.  RC_ERR_GENERAL otherwise.  
	 */
	ReturnCode packMessageIntoParcel(const Message &objMessage, Parcel &objParcel);

private:

};

#endif //#ifndef MESSAGEMANAGER_H
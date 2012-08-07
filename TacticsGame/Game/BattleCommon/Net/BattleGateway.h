#ifndef BATTLEGATEWAY_H
#define BATTLEGATEWAY_H

#include <winsock2.h>
#include <Windows.h>
#include <vector>
#include "TacticsErrors.h"
#include "SyncUtil.h"
#include "NetUtil.h"
#include "BattleConnection.h"

//Typedefs for class-specific containers
typedef std::vector<BattleConnection*> typeConnectionVec;
typedef typeConnectionVec::iterator typeConnectionVecIterator;

class BattleGateway
{
public:
	BattleGateway();
	virtual ~BattleGateway();

	virtual ReturnCode init() = 0;
	virtual void deinit() = 0;

	//Static methods
	/**
	 *	@brief Creates a parcel from the given parcel buffer.
	 *	
	 *	@param [IN] pcParcelBuffer - the parcel buffer to create a parcel from.
	 *	@param [IN] dwBufferSize - the size of the input parcel buffer
	 *	@param [OUT] objParcel - the parcel created from the given parcel buffer
	 *	@return RC_OK if creation was successful.
	 */
	static ReturnCode retrieveParcelFromBuffer(const char* pcParcelBuffer, DWORD dwBufferSize, Parcel &objParcel);

	/**
	 *	@brief Creates a parcel buffer from the given parcel.
	 *	
	 *	@param [IN] objParcel - the parcel to create the parcel buffer from
	 *	@param [OUT] pcParcelBuffer - the parcel buffer created from the given parcel
	 */
	static ReturnCode createParcelBuffer(const Parcel &objParcel, char *&pcParcelBuffer);

	/**
	 *	@brief Frees all memory associated with given parcel (in particular, memory allocated
	 *		   to the message buffer).
	 *	
	 *	@param [INOUT] objParcel - the parcel to destroy
	 *	@return RC_OK if successful.
	 */
	static void destroyParcel(Parcel &objParcel);

protected:
	//Owned properties
	bool m_bIsInitialized;
	HANDLE m_hIncomingPort;
	HANDLE m_hOutgoingPort;
	typeConnectionVec m_pobjConnectionVec;
	CriticalSectionEx CS;

	//Protected Methods
	ReturnCode AddConnection(BattleConnection* objConnection);
	ReturnCode RemoveConnection(BattleConnection* objConnection);

private:

};


#endif //#ifndef BATTLEGATEWAY_H
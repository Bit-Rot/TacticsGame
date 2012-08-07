#include "BattleGateway.h"

BattleGateway::BattleGateway()
	: m_bIsInitialized(false), m_hIncomingPort(NULL), m_hOutgoingPort(NULL)
{

}

BattleGateway::~BattleGateway()
{

}

ReturnCode BattleGateway::AddConnection(BattleConnection* objConnection)
{
	//Push the new connection in a thread-safe manner
	CS.enter();
	m_pobjConnectionVec.push_back(objConnection);	
	CS.leave();

	//Success!
	return RC_OK;
}

ReturnCode BattleGateway::RemoveConnection(BattleConnection* objConnection)
{
	//Assume we won't find the connection, update to RC_OK if we do.
	ReturnCode rc = RC_ERR_NOT_FOUND;

	CS.enter();

	//Iterate through all active connections
	typeConnectionVecIterator iter;
	for(iter = m_pobjConnectionVec.begin(); iter != m_pobjConnectionVec.end(); ++iter)
	{
		//Is this the right connection?
		if(*iter == objConnection)
		{
			//Remove the connection from the list of active connections
			m_pobjConnectionVec.erase(iter);

			//Deallocate memory used by connection
			delete objConnection;
			objConnection = NULL;

			//Note that we found the connection
			rc = RC_OK;
			break;
		}
	}

	CS.leave();

	//Return either RC_OK or RC_ERR_NOT_FOUND
	return rc;
}

//TODO: Catch exceptions thrown here
ReturnCode BattleGateway::retrieveParcelFromBuffer(const char* pcParcelBuffer, DWORD dwBufferSize, Parcel &objParcel)
{
	int index = 0;
	if(dwBufferSize >= sizeof(objParcel.size))
	{
		memcpy(&(objParcel.size), pcParcelBuffer + index, sizeof(objParcel.size));
		index += sizeof(objParcel.size);
	}
	if(dwBufferSize >= sizeof(objParcel.message_type) + index)
	{
		memcpy(&(objParcel.message_type), pcParcelBuffer + index, sizeof(objParcel.message_type));
		index += sizeof(objParcel.message_type);
	}
	if(dwBufferSize >= sizeof(objParcel.message_size) + index)
	{
		memcpy(&(objParcel.message_size), pcParcelBuffer + index, sizeof(objParcel.message_size));
		index += sizeof(objParcel.message_size);

		//Allocate memory for the message
		objParcel.message_buff = new char[objParcel.message_size];
	}
	if(dwBufferSize >= sizeof(char)*objParcel.message_size + index)
	{
		memcpy(objParcel.message_buff, pcParcelBuffer + index, sizeof(char)*objParcel.message_size);
		index += objParcel.message_size;
	}

	//Success!
	return RC_OK;
}

ReturnCode BattleGateway::createParcelBuffer(const Parcel &objParcel, char *&pcParcelBuffer)
{
	//Allocate memory for our parcel buffer
	pcParcelBuffer = new char[objParcel.size];

	//Copy over parcel size
	char* index = pcParcelBuffer;
	memcpy(index, &(objParcel.size), sizeof(objParcel.size));
	index += sizeof(objParcel.size);

	//Copy over parcel message type
	memcpy(index, &(objParcel.message_type), sizeof(objParcel.message_type));
	index += sizeof(objParcel.message_type);

	//Copy over parcel message size
	memcpy(index, &(objParcel.message_size), sizeof(objParcel.message_size));
	index += sizeof(objParcel.message_size);

	//Copy over parcel message
	memcpy(index, objParcel.message_buff, objParcel.message_size);
	index += objParcel.message_size;

	//Success!
	return RC_OK;
}

void BattleGateway::destroyParcel(Parcel &objParcel)
{
	if(objParcel.message_buff)
	{
		delete [] objParcel.message_buff;
		objParcel.message_buff = NULL;
	}
}
#include "ChangeEntityPositionMessage.h"
#include "EntityManager.h"


ChangeEntityPositionMessage::ChangeEntityPositionMessage() :
	m_uiEntityId(0), m_iX(0), m_iY(0)
{
}

ChangeEntityPositionMessage::~ChangeEntityPositionMessage()
{
}

ReturnCode ChangeEntityPositionMessage::init(char* pcBuffer, DWORD dwBufferSize)
{
	//TODO: Uncomment when the base Message class has data that needs to be pushed to the buffer via initBaseMessage
	//Build base message properties from buffer
	//DWORD dwBytesUnpacked = 0;
	//RET_RC_IF_FAILED(initBaseMessage(pcBuffer, dwBufferSize, dwBytesUnpacked), RC_ERR_GENERAL);

	//Unpack m_iX
	DWORD dwBytesUnpacked = 0;
	if(sizeof(m_iX) > dwBufferSize)
		return RC_ERR_GENERAL;
	memcpy(&m_iX, pcBuffer, sizeof(m_iX));
	dwBytesUnpacked += sizeof(m_iX);

	//Unpack m_iY
	if(dwBytesUnpacked + sizeof(m_iY) > dwBufferSize)
		return RC_ERR_GENERAL;
	memcpy(&m_iY, pcBuffer+dwBytesUnpacked, sizeof(m_iY));
	dwBytesUnpacked += sizeof(m_iY);

	//Unpack m_uiEntityId
	if(dwBytesUnpacked + sizeof(m_uiEntityId) > dwBufferSize)
		return RC_ERR_GENERAL;
	memcpy(&m_uiEntityId, pcBuffer+dwBytesUnpacked, sizeof(m_uiEntityId));
	dwBytesUnpacked += sizeof(m_uiEntityId);

	//Note successful initialization and return successfully
	m_bIsWellFormed = true;
	return RC_OK;
}

ReturnCode ChangeEntityPositionMessage::execute()
{
	//Get entity by id
	Entity* pobjEntity;
	RET_RC_IF_FAILED(EntityManager::getInstance()->getEntityById(m_uiEntityId, pobjEntity), RC_ERR_GENERAL);

	//Request a position change
	return EntityManager::getInstance()->getMessageHandler()->requestChangeEntityPosition(pobjEntity, m_iX, m_iY);
}

MessageClassId::MessageClassId ChangeEntityPositionMessage::getClassId() const
{
	return MessageClassId::ChangeEntityPositionMessageClassId;
}

ReturnCode ChangeEntityPositionMessage::getMessageBuffer(char** pcBuffer, DWORD &dwSize) const
{
	//Get the total size necessary to create the message buffer
	dwSize = getBaseMessageSize();
	dwSize += getMessageSize();

	//Allocate memory for the buffer
	char* pcRetBuffer = new char[dwSize];

	//Start packing data
	DWORD dwBytesPacked = 0;
	memcpy(pcRetBuffer, &m_iX, sizeof(m_iX));
	dwBytesPacked += sizeof(m_iX);
	memcpy(pcRetBuffer+dwBytesPacked, &m_iY, sizeof(m_iY));
	dwBytesPacked += sizeof(m_iY);
	memcpy(pcRetBuffer+dwBytesPacked, &m_uiEntityId, m_uiEntityId);
	dwBytesPacked += sizeof(m_uiEntityId);

	//Push message buffer to output and return successfully
	*pcBuffer = pcRetBuffer;
	return RC_OK;
}

DWORD ChangeEntityPositionMessage::getMessageSize() const
{
	return sizeof(m_iX) + sizeof(m_iY) + sizeof(m_uiEntityId);
}


#include "PingMessage.h"

//TODO: KILL ME!  KILL ME WITH FIRE!
#include <stdio.h>


PingMessage::PingMessage()
	: m_iSomeInt(127), m_pcSomeString(NULL)
{
	//Create//allocate our test string
	m_pcSomeString = new char[6];
	strcpy_s(m_pcSomeString, 6, "Ping!");

	//Set test string size
	m_dwSomeStringSize = 6;
}

PingMessage::~PingMessage()
{
	if(m_pcSomeString)
	{
		delete[] m_pcSomeString;
		m_pcSomeString = NULL;
	}
}

ReturnCode PingMessage::execute()
{
	//Check if message has been properly constructed
	if(!m_bIsWellFormed)
		return RC_ERR_INVALID_STATE;

	//TODO: Implement <i>ACTUAL</i> execute logic
	if(m_pcSomeString)
		printf("%s", m_pcSomeString);

	return RC_ERR_NOT_IMPLEMENTED;
}

MessageClassId::MessageClassId PingMessage::getClassId() const
{
	return MessageClassId::PingMessageClassId;
}

ReturnCode PingMessage::init(char* pcBuffer, DWORD dwBufferSize)
{
	//TODO: Uncomment when the base Message class has data that needs to be pushed to the buffer via initBaseMessage
	//Build base message properties from buffer
	//DWORD dwBytesUnpacked = 0;
	//RET_RC_IF_FAILED(initBaseMessage(pcBuffer, dwBufferSize, dwBytesUnpacked), RC_ERR_GENERAL);

	//Unpack m_iSomeInt
	DWORD dwBytesUnpacked = 0;
	if(dwBytesUnpacked + sizeof(m_iSomeInt) > dwBufferSize)
		return RC_ERR_GENERAL;
	memcpy(&m_iSomeInt, pcBuffer+dwBytesUnpacked, sizeof(m_iSomeInt));
	dwBytesUnpacked += sizeof(m_iSomeInt);

	//Unpack m_dwSomeStringSize
	if(dwBytesUnpacked + sizeof(m_dwSomeStringSize) > dwBufferSize)
		return RC_ERR_GENERAL;
	memcpy(&m_dwSomeStringSize, pcBuffer+dwBytesUnpacked, sizeof(m_dwSomeStringSize));
	dwBytesUnpacked += sizeof(m_dwSomeStringSize);

	//Unpack m_pcSomeString
	if(dwBytesUnpacked + m_dwSomeStringSize > dwBufferSize)
		return RC_ERR_GENERAL;
	memcpy(m_pcSomeString, pcBuffer+dwBytesUnpacked, m_dwSomeStringSize);
	dwBytesUnpacked += m_dwSomeStringSize;

	//Note successful initialization and return successfully
	m_bIsWellFormed = true;
	return RC_OK;

}

DWORD PingMessage::getMessageSize() const
{
	//Add 'packed' property sizes together and return.
	return sizeof(m_iSomeInt) + sizeof(m_dwSomeStringSize) + m_dwSomeStringSize;
}

ReturnCode PingMessage::getMessageBuffer(char** pcBuffer, DWORD &dwSize) const
{
	//Get the total size necessary to create the message buffer
	dwSize = getBaseMessageSize();
	dwSize += getMessageSize();

	//Allocate memory for the buffer
	char* pcRetBuffer = new char[dwSize];

	//Start packing data
	DWORD dwBytesPacked = 0;
	memcpy(pcRetBuffer+dwBytesPacked, &m_iSomeInt, sizeof(m_iSomeInt));
	dwBytesPacked += sizeof(m_iSomeInt);
	memcpy(pcRetBuffer+dwBytesPacked, &m_dwSomeStringSize, sizeof(m_dwSomeStringSize));
	dwBytesPacked += sizeof(m_dwSomeStringSize);
	memcpy(pcRetBuffer+dwBytesPacked, m_pcSomeString, m_dwSomeStringSize);
	dwBytesPacked += m_dwSomeStringSize;

	//Push message buffer to output and return successfully
	*pcBuffer = pcRetBuffer;
	return RC_OK;
}




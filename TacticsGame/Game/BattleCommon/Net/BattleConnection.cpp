#include "BattleConnection.h"
#include "NetUtil.h"

BattleConnection::BattleConnection()
	: m_bTransferInProgress(false), m_dwCurrentParcelSize(0), m_dwBytesTransferred(0), m_pcParcelBuffer(NULL),
	  m_dwBytesInSizeBuffer(0)
{
	//Initialize and zero overlapped
	m_pobjOverlapped = new WSAOVERLAPPED;
	ZeroMemory(m_pobjOverlapped, sizeof(WSAOVERLAPPED));

	//Initialize and zero wsa buffer
	m_pobjWsaBuffer = new WSABUF;

	//Set wsa buffer data members
	m_pobjWsaBuffer->buf = m_cTempBuffer;
	m_pobjWsaBuffer->len = MAX_CONNECTION_BUFFER_LEN;
	ZeroMemory(m_cTempBuffer, MAX_CONNECTION_BUFFER_LEN);

	//Allocate memory to the size buffer
	m_pcSizeBuffer = new char[sizeof(typeParcelSize)];
	ZeroMemory(m_pcSizeBuffer, sizeof(typeParcelSize));
}

BattleConnection::~BattleConnection()
{
	//Delete members allocated dynamically in constructor
	delete m_pobjOverlapped;
	delete m_pobjWsaBuffer;
	delete m_pcSizeBuffer;

	//Close member socket
	NetUtil::CloseSocket(m_hSocket);
}

ReturnCode BattleConnection::appendWsaBuffToParcelBuff(DWORD dwBytesToAppend)
{
	//Do some boundary checking
	if(m_dwBytesTransferred + dwBytesToAppend > m_dwCurrentParcelSize)
	{
		//TODO: Enhance error handling here.  This is REALLY bad, should be logged.
		return RC_ERR_GENERAL;
	}

	//Copy over the memory
	memcpy(m_pcParcelBuffer + m_dwBytesTransferred, m_pobjWsaBuffer->buf, dwBytesToAppend);

	//Success!
	return RC_OK;
}

ReturnCode BattleConnection::appendSizeBufftoParcelBuff()
{
	//Nothing to see here, move along folks.
	if(!m_dwBytesInSizeBuffer)
		return RC_OK;

	//I said move along, nothing to see here. (Size buffer must be appended first!)
	if(m_dwBytesTransferred)
		return RC_ERR_INVALID_STATE;
	
	//Okay, I lied, there's totally something to see here.
	memcpy(m_pcParcelBuffer, m_pcSizeBuffer, m_dwBytesInSizeBuffer);
	return RC_OK;
}

ReturnCode BattleConnection::allocateParcelBuffer(typeParcelSize tpsSize)
{
	//Check to make sure we haven't already allocated memory to the parcel buffer.
	if(m_pcParcelBuffer)
		return RC_ERR_INVALID_STATE;

	//Allocate memory to the parcel buffer
	m_pcParcelBuffer = new char[tpsSize];

	//Success!  Set parcel size and return successfully.
	m_dwCurrentParcelSize = tpsSize;
	return RC_OK;
}

ReturnCode BattleConnection::resetParcelBuffer()
{
	//Check to make sure we haven't already reset the parcel buffer.
	if(!m_pcParcelBuffer)
		return RC_ERR_INVALID_STATE;

	delete[] m_pcParcelBuffer;
	m_pcParcelBuffer = NULL;
	return RC_OK;
}

void BattleConnection::resetSizeBuffer()
{
	//Zero memory out and return successfully
	ZeroMemory(m_pcSizeBuffer, sizeof(typeParcelSize));
}

void BattleConnection::pullParcelSizeFromBuffers(typeParcelSize &tpsSize)
{
	//Clear output value before memcpy calls.
	tpsSize = 0;

	//Pull all bytes from the size buffer, if any
	if(m_dwBytesInSizeBuffer)
		memcpy(&tpsSize, m_pcSizeBuffer, m_dwBytesInSizeBuffer);

	//Pull remaining bytes from the wsa buffer
	memcpy((char*)&tpsSize + m_dwBytesInSizeBuffer, m_pobjWsaBuffer->buf, sizeof(typeParcelSize) - m_dwBytesInSizeBuffer);
}

ReturnCode BattleConnection::moveRemainingBytesToFrontOfBuffer(DWORD dwBytesTransferred, DWORD dwBytesAppended)
{
	//Check bounds for initial copy, return immediately if no work needs to be done
	if(dwBytesTransferred <= dwBytesAppended)
		return RC_ERR_INVALID_STATE;

	//Check for bad input.
	if(dwBytesTransferred > MAX_CONNECTION_BUFFER_LEN)
		return RC_ERR_INVALID_ARGS;

	//Take note of new parcel size
	DWORD dwBytesToMove = dwBytesTransferred - dwBytesAppended;

	//Copy over bytes from next package to front of parcel buffer
	memcpy(m_pobjWsaBuffer->buf, m_pobjWsaBuffer->buf + dwBytesAppended, dwBytesToMove);

	//Zero out remaining bytes
	ZeroMemory(m_pobjWsaBuffer->buf + dwBytesToMove, MAX_CONNECTION_BUFFER_LEN - dwBytesToMove);

	//Success!
	return RC_OK;
}

void BattleConnection::clearConnection()
{
	resetBytesTransferred();
	resetParcelBuffer();
	setCurrentParcelSize(0);
	resetSizeBuffer();
	setBytesInSizeBuffer(0);
	setTransferInProgress(false);
}
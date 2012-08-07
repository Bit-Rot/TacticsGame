#ifndef BATTLECONNECTION_H
#define BATTLECONNECTION_H

#include <winsock2.h>
#include <Windows.h>
#include "TacticsErrors.h"
#include "Parcel.h"

#define MAX_CONNECTION_BUFFER_LEN 256
/**
 *	@brief 
 *	The <pre>BattleConnection</pre> class represents a single remote connection, and is used
 *	primarily to store information necessary to facilitate communication between the current 
 *	process and a remote agent.
 */
class BattleConnection
{

public:
	//C'tors and D'tor
	BattleConnection();
	~BattleConnection();
	
	//Public methods
	bool isTransferInProgress(){return m_bTransferInProgress;}
	void addBytesTransferred(DWORD dwBytes){m_dwBytesTransferred += dwBytes;}
	void resetBytesTransferred(){m_dwBytesTransferred = 0;}
	ReturnCode allocateParcelBuffer(typeParcelSize tpsSize);
	ReturnCode resetParcelBuffer();
	ReturnCode appendWsaBuffToParcelBuff(DWORD dwBytesToAppend);
	void pullParcelSizeFromBuffers(typeParcelSize &tpsSize);
	void resetSizeBuffer();
	ReturnCode appendSizeBufftoParcelBuff();
	ReturnCode moveRemainingBytesToFrontOfBuffer(DWORD dwBytesTransferred, DWORD dwBytesAppended);
	void clearConnection();

	//Setters
	void setTransferInProgress(bool bTransferState){m_bTransferInProgress = bTransferState;}
	void setSocket(SOCKET hSocket){m_hSocket = hSocket;}
	void setCurrentParcelSize(DWORD dwSize){m_dwCurrentParcelSize = dwSize;}
	void setBytesInSizeBuffer(DWORD dwBytesInSizeBuffer){m_dwBytesInSizeBuffer = dwBytesInSizeBuffer;}

	//Getters
	SOCKET getSocket(){return m_hSocket;}
	DWORD getCurrentParcelSize(){return m_dwCurrentParcelSize;}
	DWORD getBytesTransferred(){return m_dwBytesTransferred;}
	WSAOVERLAPPED* getOverlapped(){return m_pobjOverlapped;}
	WSABUF* getWsaBuffer(){return m_pobjWsaBuffer;}
	char* getParcelBuffer(){return m_pcParcelBuffer;}
	char* getSizeBuffer(){return m_pcSizeBuffer;}
	DWORD getBytesInSizeBuffer(){return m_dwBytesInSizeBuffer;}
	DWORD getBytesLeftInParcel(){return m_dwCurrentParcelSize - m_dwBytesTransferred;}

protected:
	SOCKET m_hSocket;
	WSAOVERLAPPED* m_pobjOverlapped;
	WSABUF* m_pobjWsaBuffer;
	char m_cTempBuffer[MAX_CONNECTION_BUFFER_LEN];
	bool m_bTransferInProgress;
	char* m_pcParcelBuffer;
	DWORD m_dwCurrentParcelSize;
	DWORD m_dwBytesTransferred;
	char* m_pcSizeBuffer;
	DWORD m_dwBytesInSizeBuffer;
};

#endif //#ifndef BATTLECONNECTION_H
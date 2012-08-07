#ifndef SYNCUTILS_H
#define SYNCUTILS_H

#include "TacticsErrors.h"
#include <windows.h>

class CriticalSectionEx
{
public:
	CriticalSectionEx(){InitializeCriticalSection(&m_cs);}
	~CriticalSectionEx(){DeleteCriticalSection(&m_cs);}
	void enter(){EnterCriticalSection(&m_cs);}
	void leave(){LeaveCriticalSection(&m_cs);}
protected:
	CRITICAL_SECTION m_cs;
};

class SyncUtil
{
public:
	static ReturnCode SyncCreateOpenMutex(bool initial_owner, const wchar_t* mutex_name, int& mutex_handle);
	static ReturnCode SyncOpenMutex(const wchar_t* mutex_name, int& mutex_handle);
	static ReturnCode SyncReleaseMutex(int mutex_handle);
	static ReturnCode SyncCloseMutex(int mutex_handle);

protected:

private:

};

#endif //#ifndef SYNCUTILS_H
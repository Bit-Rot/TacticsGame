#include "SyncUtil.h"
#include "TacticsErrors.h"

ReturnCode SyncUtil::SyncCreateOpenMutex(bool initial_owner, const wchar_t* mutex_name, int& mutex_handle)
{
	//Create Mutex
	if (!(mutex_handle = (int)CreateMutex(0, initial_owner, mutex_name)))
		return RC_OK;

	//Check for specific error
	if (GetLastError() == ERROR_ALREADY_EXISTS)
			return RC_ERR_ALREADY_EXISTS;
	
	//General error occurred
	return RC_ERR_GENERAL;
}

ReturnCode SyncUtil::SyncOpenMutex( const wchar_t* mutex_name, int& mutex_handle )
{
	//Open Mutex
	if(mutex_handle = (int)OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutex_name))
		return RC_OK;

	//General error occurred
	return RC_ERR_GENERAL;
}

ReturnCode SyncUtil::SyncReleaseMutex( int mutex_handle )
{
	//Release Mutex
	if(ReleaseMutex((HANDLE)mutex_handle))
		return RC_OK;

	//General error occurred
	return RC_ERR_GENERAL;
}

ReturnCode SyncUtil::SyncCloseMutex( int mutex_handle )
{
	//Close Mutex
	if(!CloseHandle((HANDLE)mutex_handle))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}
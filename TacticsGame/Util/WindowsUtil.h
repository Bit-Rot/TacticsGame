#ifndef WINDOWSUTIL_H
#define WINDOWSUTIL_H

#include "TacticsErrors.h"
#include <windows.h>

class WindowsUtil
{
public:
	/**
	 *	@brief Gets the number of processors available to this computer
	 *	@param [OUT] iNumProcessors - the number of processors available to this computer
	 *	@return Always returns RC_OK;
	 */
	static ReturnCode GetNumProcessors(int &iNumProcessors);

	
	/**
	*	@brief Closes the given handle and assigns it the value INVALID_HANDLE_VALUE
	*	@param [INOUT] hHandle - the socket to close
	*	@return Always returns RC_OK;
	*/
	static inline ReturnCode CloseHandle(HANDLE &hHandle)
	{
		if(INVALID_HANDLE_VALUE != hHandle)
		{
			::CloseHandle(hHandle);
			hHandle = INVALID_HANDLE_VALUE;
		}
		return RC_OK;
	}

	/**
	*	@brief Closes the given handle and assigns it the value NULL
	*	@param [INOUT] hHandle - the socket to close
	*	@return Always returns RC_OK;
	*/
	static inline ReturnCode CloseHandleNull(HANDLE &hHandle)
	{
		if(NULL != hHandle)
		{
			::CloseHandle(hHandle);
			hHandle = NULL;
		}
		return RC_OK;
	}
};

#endif //#ifndef WINDOWSUTIL_H
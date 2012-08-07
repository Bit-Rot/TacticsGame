#include "WindowsUtil.h"

ReturnCode WindowsUtil::GetNumProcessors(int &iNumProcessors)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	iNumProcessors = si.dwNumberOfProcessors;
	return RC_OK;
}


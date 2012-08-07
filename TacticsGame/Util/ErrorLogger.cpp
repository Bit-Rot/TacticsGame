#include "ErrorLogger.h"
#include <Windows.h>

#define LOG_FILE_PREFIX "TacticsLog_"
#define LOG_FIL_SUFFIX ".log"

ErrorLogger* ErrorLogger::s_instance = NULL;

ErrorLogger* ErrorLogger::getInstance()
{
	if(s_instance)
		return s_instance;
	return s_instance = new ErrorLogger();
}

void ErrorLogger::destroyInstance()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance = NULL;
	}
}

ErrorLogger::ErrorLogger()
{

}

ErrorLogger::~ErrorLogger()
{

}

ReturnCode ErrorLogger::openLogFile()
{
	//See if file already exists

	//If exists, overwrite

	//

	return RC_ERR_NOT_IMPLEMENTED;
}

void ErrorLogger::buildLogName(std::string &sLogName)
{

}

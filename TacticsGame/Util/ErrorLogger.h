#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include "SyncUtil.h"
#include <string>

class ErrorLogger
{
public:
	//Singleton stuffs
	static ErrorLogger* getInstance();
	static void destroyInstance();


protected:
	//Protected members
	CriticalSectionEx CS;

	//Protected methods
	ReturnCode openLogFile();
	void buildLogName(std::string &sLogName);
private:
	//Singleton stuffs
	static ErrorLogger* s_instance;
	ErrorLogger();
	~ErrorLogger();
};

#endif //#ifndef ERRORLOGGER_H
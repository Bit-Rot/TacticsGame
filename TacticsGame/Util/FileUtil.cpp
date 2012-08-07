#include "FileUtil.h"
#include <Windows.h>
#include <io.h>

ReturnCode FileUtil::LowLevelFileOpen(const char* pcFileName, int iFlags, int iPermissionMode, int &iOutHandle)
{
	//Open file and check for errors
	if (-1 == (iOutHandle = _open(pcFileName, iFlags, iPermissionMode)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileOpen(const char* pcFileName, const char* pcFileMode, FILE* &fFile)
{
	//Open file and check for errors
	if(NULL == (fFile = fopen(pcFileName, pcFileMode)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileClose(int iHandle)
{
	//Close the file and return appropriate error code
	int iResult = _close(iHandle);
	return (iResult == 0) ? RC_OK : RC_ERR_GENERAL;
}

ReturnCode FileUtil::LowLevelFileClose(FILE* fFile)
{
	//Close the given file handle
	return (fclose(fFile)) ? RC_ERR_GENERAL : RC_OK;
}

ReturnCode FileUtil::LowLevelFileWrite(int iHandle, const char* pcStringToWrite)
{
	//Write contents to file
	if(-1 == _write(iHandle, pcStringToWrite, strlen(pcStringToWrite)))
		return RC_ERR_GENERAL;
	
	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileWrite(FILE* fFile, const char* pcStringToWrite)
{
	//Write contents to file
	size_t count = strlen(pcStringToWrite);
	if(count > fwrite(pcStringToWrite, sizeof(char), count, fFile))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileRead(int iHandle, char* pcBuffer, size_t uiBytesToRead, size_t &uiBytesRead)
{
	//Read file content into buffer
	if(-1 == (uiBytesRead = _read(iHandle, pcBuffer, uiBytesToRead)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileRead(FILE* fFile, char* pcBuffer, size_t uiBufferSize, size_t uiBytesToRead, size_t &uiBytesRead)
{
	//Read file content into buffer
	if(uiBytesToRead > (uiBytesRead = fread_s(pcBuffer, uiBufferSize, sizeof(char), uiBytesToRead, fFile)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileSeek(int iHandle, long lOffset, int iOrigin, int &iNewPos)
{
	//Seek to given position
	if(-1 == (iNewPos = lseek(iHandle, lOffset, iOrigin)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

ReturnCode FileUtil::LowLevelFileSeek(FILE* fFile, long lOffset, int iOrigin, int &iNewPos)
{
	//Seek to given position
	if(-1 == (iNewPos = fseek(fFile, lOffset, iOrigin)))
		return RC_ERR_GENERAL;

	//Success!
	return RC_OK;
}

#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "TacticsErrors.h"
#include <string>

class FileUtil
{
public:
	/**
	 *	@brief Opens/creates a file.
	 *	@param [IN] pcFileName - the name of the file to open
	 *	@param [IN] iFlags - op flags.  See http://msdn.microsoft.com/en-us/library/z0kc8e3z(v=vs.71).aspx for more info.
	 *	@param [IN] iPermissionMode - Permission mode.  See http://msdn.microsoft.com/en-us/library/z0kc8e3z(v=vs.71).aspx for more info.
	 *	@param [OUT] iOutHandle - A file descriptor for the opened file.
	 *	@return RC_OK if open/create was successful, RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileOpen(const char* pcFileName, int iFlags, int iPermissionMode, int &iOutHandle );

	/**
	 *	@brief Opens/creates a file.
	 *	@param [IN] pcFileName - the name of the file to open
	 *	@param [IN] pcFileMode - the mode to open the file under.  See http://msdn.microsoft.com/en-us/library/yeby3zcb(v=vs.71).aspx for more info.
	 *	@param [OUT] fFile - a file pointer to the opened file.
	 *	@return RC_OK if the file was opened/created successfully.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileOpen(const char* pcFileName, const char* pcFileMode, FILE* &fFile);

	/**
	 *	@brief Closes a file descriptor
	 *	@param [IN] iHandle - the file descriptor to be closed
	 *	@return RC_OK if the given file descriptor was closed successfully.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileClose(int iHandle);

	/**
	 *	@brief Closes a file
	 *	@param [IN] fFile - a pointer to the file to be closed
	 *	@return RC_OK if the given file was closed successfully.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileClose(FILE* fFile);

	/**
	 *	@brief Writes a given string to a file
	 *	@param [IN] iHandle - the file descriptor for the file to be written to
	 *	@param [IN] pcStringToWrite - the string to write to the file
	 *	@return RC_OK if the string was written successfully.  RC_ERR_GENERAL otherwise
	 */
	static ReturnCode LowLevelFileWrite(int iHandle, const char* pcStringToWrite);

	/**
	 *	@brief Writes a given string to a file
	 *	@param [IN] fFile - a pointer to the file to be written to
	 *	@param [IN]
	 */
	static ReturnCode LowLevelFileWrite(FILE* fFile, const char* pcStringToWrite);

	/**
	 *	@brief Reads a number of characters from a file.
	 *	@param [IN] iHandle - the file descriptor for the file to be read from
	 *	@param [IN] pcBuffer - a pointer to the buffer which will hold the read characters
	 *	@param [IN] uiBytesToRead - the number of characters to read from the file
	 *	@param [OUT] uiBytesRead - the number of characters read from the file
	 *	@return RC_OK if the read operation was successful.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileRead(int iHandle, char* pcBuffer, size_t uiBytesToRead, size_t &uiBytesRead);

	/**
	 *	@brief Reads a number of characters from a file.
	 *	@param [IN] fFile - a pointer to the file to be read from
	 *	@param [IN] pcBuffer - a pointer to the buffer which will hold the read characters
	 *	@param [IN] uiBufferSize - the number of bytes allocated to pcBuffer
	 *	@param [IN] uiBytesToRead - the number of characters to read from the file
	 *	@param [OUT] uiBytesRead - the number of characters read from the file
	 *	@return RC_OK if the read operation was successful.  RC_ERR_GENERAL if the number of bytes read is less than specified 
	 *			by uiBytesToRead.
	 */
	static ReturnCode LowLevelFileRead(FILE* fFile, char* pcBuffer, size_t uiBufferSize, size_t uiBytesToRead, size_t &uiBytesRead);

	/**
	 *	@brief Seek to a desired position within the given file.
	 *	@param [IN] iHandle - the file descriptor for the file we wish to seek through
	 *	@param [IN] lOffset - the number of characters beyond the origin through which we wish to seek
	 *	@param [IN] iOrigin - the starting point from which we wish to seek
	 *	@param [OUT] iNewPos - the position within the file lOffset characters from iOrigin.
	 *	@return RC_OK if the seek operation was successful, RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileSeek(int iHandle, long lOffset, int iOrigin, int &iNewPos);
	
	/**
	 *	@brief Seek to a desired position within the given file.
	 *	@param [IN] fFile - a pointer to the file we wish to seek through
	 *	@param [IN] lOffset - the number of characters beyond the origin through which we wish to seek
	 *	@param [IN] iOrigin - the starting point from which we wish to seek
	 *	@param [OUT] iNewPos - the position within the file lOffset characters from iOrigin.
	 *	@return RC_OK if the seek operation was successful, RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode LowLevelFileSeek(FILE* fFile, long lOffset, int iOrigin, int &iNewPos);

	/**
	 *	@brief Creates a new file
	 *	@param [IN] wsPath - Path to the file to be created
	 *	@param [IN] wsContext - The file's context
	 *	@param [IN] wsMode - the desired r/w/a mode.  See http://msdn.microsoft.com/en-us/library/yeby3zcb(v=vs.71).aspx for more info.
	 *	@return RC_OK if successful.  RC_ERR_GENERAL otherwise.
	 */
	static ReturnCode CreateNewFile(const std::string &wsPath, const std::string &wsMode);
protected:

private:

};

#endif //#ifndef FILEUTIL_H
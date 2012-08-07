#ifndef DIRECTUTILS_H
#define DIRECTUTILS_H

#include "TacticsErrors.h"

#include <d3dx10.h>
#include <dxerr.h>
#include <list>
#include <string>

template<typename T>
inline T isValid(T arg, const char* errorMsg=0) {
	if (!arg->IsValid()) {
		DPThrow(errorMsg ? errorMsg : "Type is not valid");
	}
	return arg;
}

class DirectUtils 
{
public:
	/**
	 *	@brief Returns the number of available adapters on the system.
	 *	@return RC_OK if successful.  RC_ERR_INVALID_ARGS if input IDXGIFactory is invalid.
	 */
	static ReturnCode GetNumAdapters(IDXGIFactory *dxgiFactory, int &iNumAdapters);

	/**
	 *	@brief Returns the number of outputs associated with a given adapter.
	 *	@return RC_OK if successful.  RC_ERR_INVALID_ARGS if input IDXGIAdapter is invalid.
	 */
	 static ReturnCode GetNumOutputsForAdapter(IDXGIAdapter *dxgiAdapter, int &iNumOutputs);

	/**
	*	@brief Returns the number of outputs associated with all adapters
	*	@return RC_OK if successful.  RC_ERR_INVALID_ARGS if input IDXGIFactory is invalid.  RC_ERR_GENERAL otherwise.
	 */
	 static ReturnCode GetNumOutputs(IDXGIFactory *dxgiFactory, int &iNumOutputsTotal);

	 /**
	  *	@brief Returns a list of supported display modes for the given adapter.
	  *	@param objDisplayModeList - the list of supported display modes.
	  *	@return RC_OK if successful.  RC_ERR_INVALID_ARGS if input IDXGIOutput is invalid. RC_ERR_GENERAL otherwise.
	  */
	 static ReturnCode GetSupportedDisplayModes(IDXGIOutput *dxgiOutput, DXGI_FORMAT dxgiFormat, std::list<DXGI_MODE_DESC*> &objDisplayModeList);

	 /**
	  *	@brief Returns a world transform matrix dependent only upon the x,y,z 
	  *			coordinates of the object at hand, relative to the origin of the
	  *			world coordinate system.  Note that the object's local x-, y-, and 
	  *			z-axis will be assumed to be parallel to the worlds x-, y-, and z-axis,
	  *			respectively.
	  *	@param x - the x coordinate of the object at hand.
	  *	@param y - the y coordinate of the object at hand.
	  *	@param z - the z coordinate of the object at hand.
	  *	@param worldMatrix - the world transform matrix.
	  *	@return RC_OK, always succeeds.
	  */
	 static ReturnCode BuildWorldMatrixWithSameOrientation(float x, float y, float z, D3DXMATRIX &worldMatrix);

	 /**
	  *	@brief Retrieves an ID3D10Effect object defined in the given file.
	  *	@param wsFileName - the name of the file which contains the desired effect
	  *	@param pobjEffect - the output effect file
	  *	@return RC_OK if successful.  RC_ERR_GENERAL otherwise.
	  */
	 static ReturnCode BuildFxFromFile(ID3D10Device* pobjDevice, const std::wstring &wsFileName, ID3D10Effect* &pobjEffect);

	 /**
	  *	@brief Translates HRESULT error codes into ReturnCodes, as follows:
	  *			<ul>
	  *				<li>D3D10_ERROR_FILE_NOT_FOUND --> RC_ERR_NOT_FOUND</li>
	  *				<li>D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS --> RC_ERR_GENERAL</li>
	  *				<li>D3DERR_INVALIDCALL --> RC_ERR_INVALID_ARGS</li>
	  *				<li>D3DERR_WASSTILLDRAWING --> RC_ERR_GENERAL</li>
	  *				<li>E_FAIL --> RC_ERR_GENERAL</li>
	  *				<li>E_INVALIDARG --> RC_ERR_INVALID_ARGS</li>
	  *				<li>E_OUTOFMEMORY --> RC_ERR_GENERAL</li>
	  *				<li>S_FALSE --> RC_OK</li>
	  *				<li>S_OK --> RC_OK</li>
	  *			</ul>
	  *	@param hResult - the HRESULT to translate
	  *	@return See brief.
	  */
	 static ReturnCode CheckHResult(HRESULT hResult);
	 
	 /**
	  *	@brief Translates HRESULT error codes into ReturnCodes, as follows, and Traces error to debug output:
	  *			<ul>
	  *				<li>D3D10_ERROR_FILE_NOT_FOUND --> RC_ERR_NOT_FOUND</li>
	  *				<li>D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS --> RC_ERR_GENERAL</li>
	  *				<li>D3DERR_INVALIDCALL --> RC_ERR_INVALID_ARGS</li>
	  *				<li>D3DERR_WASSTILLDRAWING --> RC_ERR_GENERAL</li>
	  *				<li>E_FAIL --> RC_ERR_GENERAL</li>
	  *				<li>E_INVALIDARG --> RC_ERR_INVALID_ARGS</li>
	  *				<li>E_OUTOFMEMORY --> RC_ERR_GENERAL</li>
	  *				<li>S_FALSE --> RC_OK</li>
	  *				<li>S_OK --> RC_OK</li>
	  *			</ul>
	  *	@param hResult - the HRESULT to translate
	  *	@return See brief.
	  */
	 static ReturnCode CheckHResultAndTrace(HRESULT hResult, const char* pchFile, DWORD dwLine, wchar_t* pwcErrorMessage);
};

#endif //ifndef DIRECTUTILS_H
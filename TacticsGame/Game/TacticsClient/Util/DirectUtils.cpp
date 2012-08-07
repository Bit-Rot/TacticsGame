#include "DirectUtils.h"

#include "d3dUtil.h"


ReturnCode DirectUtils::GetNumAdapters(IDXGIFactory *dxgiFactory, int &iNumAdapters)
{
	//Check for error conditions
	if(!dxgiFactory)
		return RC_ERR_INVALID_ARGS;

	//Declare/initialize some necessary variables
	iNumAdapters = 0;
	IDXGIAdapter *dxgiAdapter;

	//Add up the number of available adapters
	while(dxgiFactory->EnumAdapters(iNumAdapters, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		++iNumAdapters;
	}

	//Return successfully
	return RC_OK;
}

ReturnCode DirectUtils::GetNumOutputsForAdapter(IDXGIAdapter *dxgiAdapter, int &iNumOutputs)
{
	//Check for error conditions
	if(!dxgiAdapter)
		return RC_ERR_INVALID_ARGS;

	//Declare/initialize some necessary variables
	iNumOutputs = 0;
	IDXGIOutput *dxgiOutput;

	//Add the number of outputs for the given adapter
	while(dxgiAdapter->EnumOutputs(iNumOutputs, &dxgiOutput) != DXGI_ERROR_NOT_FOUND)
	{
		++iNumOutputs;
	}

	//Return successfully
	return RC_OK;
}

ReturnCode DirectUtils::GetNumOutputs(IDXGIFactory *dxgiFactory, int &iNumOutputsTotal)
{
	//Check for error conditions
	if(!dxgiFactory)
		return RC_ERR_INVALID_ARGS;

	//Declare/initialize some necessary variables
	int iNumOutputsCurr = 0, iNumAdapters = 0;
	iNumOutputsTotal = 0;
	IDXGIAdapter *dxgiAdapter;
	
	//Add the number of outputs for each available adapter
	ReturnCode rc = RC_ERR_GENERAL;
	while(dxgiFactory->EnumAdapters(iNumAdapters, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		RET_RC_IF_FAILED(GetNumOutputsForAdapter(dxgiAdapter, iNumOutputsCurr), RC_ERR_GENERAL);
		
		iNumOutputsTotal += iNumOutputsCurr;
		++iNumAdapters;
	}

	//Return successfully
	return RC_OK;
}

ReturnCode DirectUtils::GetSupportedDisplayModes(IDXGIOutput *dxgiOutput, DXGI_FORMAT dxgiFormat, std::list<DXGI_MODE_DESC*> &objDisplayModeList)
{
	//Check for error conditions
	if(!dxgiOutput)
		return RC_ERR_INVALID_ARGS;

	//Get the number of display modes available
	UINT iNumModes = 0;
	if(FAILED(dxgiOutput->GetDisplayModeList(dxgiFormat, NULL, &iNumModes, NULL)))
		return RC_ERR_GENERAL;

	//Get the display modes
	DXGI_MODE_DESC *dxgiModeDesc = new DXGI_MODE_DESC[iNumModes];
	if(FAILED(dxgiOutput->GetDisplayModeList(dxgiFormat, NULL, &iNumModes, dxgiModeDesc)))
		return RC_ERR_GENERAL;

	//Package the supported display modes into a handy-dandy std::list
	size_t i;
	objDisplayModeList.clear();
	for(i=0; i<iNumModes; ++i)
	{
		//push that bad dad in there
		objDisplayModeList.push_back(&dxgiModeDesc[i]);
	}

	//Clean up our mess
	delete[] dxgiModeDesc;
	dxgiModeDesc = NULL;

	//return successfully
	return RC_OK;
}

ReturnCode DirectUtils::BuildWorldMatrixWithSameOrientation(float x, float y, float z, D3DXMATRIX &worldMatrix)
{
	worldMatrix = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 1.0f, 0.0f,
							 x, y, z, 1.0f);

	return RC_OK;
}

ReturnCode DirectUtils::BuildFxFromFile(ID3D10Device* pobjDevice, const std::wstring &wsFileName, ID3D10Effect* &pobjEffect)
{
	//Check for invalid input
	if(!pobjDevice || wsFileName.empty())
		return RC_ERR_INVALID_ARGS;

	//Build our HLSL flag
	UINT uiHlslFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined (DEBUG) | defined(_DEBUG)
	uiHlslFlags |= D3D10_SHADER_DEBUG ;
	uiHlslFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	//Create effect
	ID3D10Blob *blobErrors = NULL;
	HRESULT hr = D3DX10CreateEffectFromFile(wsFileName.c_str(),
		NULL,
		NULL,
		"fx_4_0",
		uiHlslFlags,
		NULL,
		pobjDevice,
		NULL,
		NULL,
		&pobjEffect,
		&blobErrors,
		0);

	if(RC_FAILED(CheckHResultAndTrace(hr, __FILE__, (DWORD)__LINE__, L"D3DX10CreateEffectFromFile")))
	{
		if(blobErrors)
		{
			MessageBoxA(0, (char *)blobErrors->GetBufferPointer(), NULL, 0);
			ReleaseCOM(blobErrors);
		}
	}

	//Success!
	return RC_OK;
}

ReturnCode DirectUtils::CheckHResult(HRESULT hResult)
{
	//Translate the given error code
	ReturnCode rc;
	switch(hResult)
	{
	case D3D10_ERROR_FILE_NOT_FOUND:
		rc = RC_ERR_NOT_FOUND;
		break;
	case D3DERR_INVALIDCALL:
	case E_INVALIDARG:
		rc = RC_ERR_INVALID_ARGS;
		break;
	case S_FALSE:
	case S_OK:
		rc = RC_OK;
		break;
	default:
		rc = RC_ERR_GENERAL;
	}

	//Return the spiffy new ReturnCode
	return rc;
}

ReturnCode DirectUtils::CheckHResultAndTrace(HRESULT hResult, const char* pchFile, DWORD dwLine, wchar_t* pwcErrorMessage)
{
#if defined(DEBUG) || defined(_DEBUG)
	if(FAILED(hResult))
		DXTrace(pchFile, dwLine, hResult, pwcErrorMessage, true);
#endif

	return CheckHResult(hResult);
}
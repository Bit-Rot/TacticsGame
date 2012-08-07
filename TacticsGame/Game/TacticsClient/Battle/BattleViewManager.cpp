#include "BattleViewManager.h"
#include "DirectUtils.h"

BattleViewManager* BattleViewManager::m_instance = NULL;

BattleViewManager::BattleViewManager()
	: m_pobjLandscapeView(NULL), m_pobjWvpVar(NULL), m_pobjFx(NULL), 
	m_pobjFxTech(NULL), m_pobjInputLayout(NULL), m_pobjCamera(NULL)
{}

BattleViewManager::~BattleViewManager()
{
	CS.enter();

	//Release COM objects
	ReleaseCOM(m_pobjFx);
	ReleaseCOM(m_pobjInputLayout);

	//Delete Landscape
	if(m_pobjLandscapeView)
	{
		delete m_pobjLandscapeView;
		m_pobjLandscapeView = NULL;
	}

	//Destroy Entities
	std::set<EntityView*>::iterator iter;
	for(iter = m_pobjEntityViewSet.begin(); iter != m_pobjEntityViewSet.end(); iter++)
	{
		if(*iter)
		{
			delete *iter;
		}
	}

	CS.leave();
}

BattleViewManager* BattleViewManager::getInstance()
{
	if(m_instance)
		return m_instance;
	return m_instance = new BattleViewManager();
}

void BattleViewManager::destroyInstance()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

//TODO: Organize this better, this is messy as shit
ReturnCode BattleViewManager::init(ID3D10Device* &pobjDevice, const Camera &pobjCamera)
{
	//Set the D3D10 device!
	m_pobjDevice = pobjDevice;
	m_pobjCamera = &pobjCamera;

	//Initialize miscellaneous data
	D3DXMatrixIdentity(&m_mProj);

	//Get default effects file
	//TODO: Make an actual effects file for our landscape
	RET_IF_FAILED(DirectUtils::BuildFxFromFile(m_pobjDevice, L"Effects\\default.fx", m_pobjFx));

	//Get our WVP matrix constant buffer variable
	m_pobjWvpVar = m_pobjFx->GetVariableByName("gWVP")->AsMatrix();
	if(!m_pobjWvpVar->IsValid()) 
		return RC_ERR_NOT_FOUND;

	//Create our input element description
	D3D10_INPUT_ELEMENT_DESC objInputDescArray[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	//Get our pass description
	m_pobjFxTech = m_pobjFx->GetTechniqueByName("DefaultTech");
	D3D10_PASS_DESC objPassDesc;
	HRESULT hr;
	hr = m_pobjFxTech->GetPassByIndex(0)->GetDesc(&objPassDesc);
	RET_IF_FAILED(DirectUtils::CheckHResultAndTrace(hr, __FILE__, (DWORD)__LINE__, L"Failed to get technique description."));

	//Get our input layout
	hr = m_pobjDevice->CreateInputLayout(objInputDescArray, 2, objPassDesc.pIAInputSignature, objPassDesc.IAInputSignatureSize, &m_pobjInputLayout);
	RET_IF_FAILED(DirectUtils::CheckHResultAndTrace(hr, __FILE__, (DWORD)__LINE__, L"Failed to create input layout."));

	//Success!
	return RC_OK;
}

ReturnCode BattleViewManager::draw()
{
	//Make sure we have everything we need to draw
	if(!m_pobjLandscapeView)
		return RC_ERR_INVALID_STATE;

	//Clear the slate for any changes that may have been made to the DS buffer, blend states, etc.
	restoreDefaultStates();

	//Get the technique description for the current effect file so we can get the number of passes.
	D3D10_TECHNIQUE_DESC objTechDesc;
	HR(m_pobjFxTech->GetDesc(&objTechDesc));

	//Grab the view Matrix once so we don't have to do it a bunch of times in a loop
	D3DXMATRIX mViewProjMatrix;
	m_pobjCamera->getViewMatrix(mViewProjMatrix);
	mViewProjMatrix *= m_mProj;

	//Draw relevant objects, one pass at a time.
	D3DXMATRIX mWvp;
	D3DXMatrixIdentity(&mWvp);
	ReturnCode rc = RC_OK;
	for(UINT i=0; i < objTechDesc.Passes; ++i)
	{

		//Calculate and set the view/projection matrix for the landscape
		//Note that we don't worry about the world-transform since Landscape should be static at origin
		m_pobjWvpVar->SetMatrix((float*)&mViewProjMatrix);
		m_pobjFxTech->GetPassByIndex(i)->Apply(NULL);

		//Draw the landscape
		//TODO: Implement some sort of debug tracing here.
		if(RC_FAILED(m_pobjLandscapeView->draw()))
			rc = RC_ERR_GENERAL;

		//Draw all entities
		typeEntityViewIterator iter;
		for(iter = m_pobjEntityViewSet.begin(); iter != m_pobjEntityViewSet.end(); ++iter)
		{
			//Recalculate WVP matrix
			if(RC_SUCCESS((*iter)->getWorldMatrix(mWvp)))
			{
				mWvp *= mViewProjMatrix;
				m_pobjWvpVar->SetMatrix((float*)&mWvp);
				m_pobjFxTech->GetPassByIndex(i)->Apply(NULL);

				//Draw the EntityView
				if(RC_FAILED((*iter)->draw()))
					rc = RC_ERR_GENERAL;
			}
			else
			{
				rc = RC_ERR_GENERAL;
			}
		}

		//TODO: Draw HUD
	}

	return rc;
}

void BattleViewManager::onResize(int iClientWidth, int iClientHeight)
{
	//Determine aspect ratio
	float aspect = (float)iClientWidth/iClientHeight;

	//Pull camera properties
	float fFovY = m_pobjCamera->getFovY();
	float fNear = m_pobjCamera->getNearPlane();
	float fFar = m_pobjCamera->getFarPlane();

	//Calculate projection matrix
	D3DXMatrixPerspectiveFovLH(&m_mProj, fFovY, aspect, fNear, fFar);
}

void BattleViewManager::restoreDefaultStates()
{
	// Restore default states, input layout and primitive topology because mFont->DrawText changes them.  
	// Note that we can restore the default states by passing null.
	m_pobjDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pobjDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	m_pobjDevice->IASetInputLayout(m_pobjInputLayout);
	m_pobjDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//TODO: Update with client/server implementation in mind
ReturnCode BattleViewManager::addEntityView(EntityView* pobjEntityView)
{
	CS.enter();
	m_pobjEntityViewSet.insert(pobjEntityView);
	CS.leave();
	return RC_OK;
}

//TODO: Update with client/server implementation in mind
ReturnCode BattleViewManager::removeEntityView(EntityView* pobjEntityView)
{
	CS.enter();
	m_pobjEntityViewSet.erase(pobjEntityView);
	CS.leave();
	return RC_OK;
}
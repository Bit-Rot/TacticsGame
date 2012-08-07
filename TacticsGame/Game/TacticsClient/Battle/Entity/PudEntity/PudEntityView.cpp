#include "PudEntityView.h"
#include "Vertex.h"
#include "d3dUtil.h"
#include "BattleViewManager.h"
#include "DirectUtils.h"
#include "LandscapeView.h"
#include "GridUnit.h"

PudEntityView::PudEntityView()
	: EntityView()
{}

PudEntityView::~PudEntityView()
{}

ReturnCode PudEntityView::draw()
{
	//Get device and ensure a valid state
	ID3D10Device* pobjDevice = BattleViewManager::getInstance()->getDevice();
	if(!pobjDevice || !m_pobjVertexBuffer || !m_pobjIndexBuffer)
		return RC_ERR_INVALID_STATE;
	
	UINT uiOffset = 0;
	pobjDevice->IASetVertexBuffers(0, 1, &m_pobjVertexBuffer, &m_uiStride, &uiOffset);
	pobjDevice->IASetIndexBuffer(m_pobjIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Draw!
	pobjDevice->DrawIndexed(m_uiNumIndices, 0, 0);

	return RC_OK;
}

PudEntity* PudEntityView::getEntity()
{
	return (PudEntity*)m_pobjEntity;
}

ReturnCode PudEntityView::buildViewForPudEntity(PudEntityView &objEntityView)
{
	//Build our vertices
	int iNumVertices = 5;
	Vertex objVertexArray[] =
	{
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), BLACK},
		{D3DXVECTOR3(1.0f, -1.0f, -1.0f), BLACK},
		{D3DXVECTOR3(-1.0f, -1.0f, 1.0f), BLACK},
		{D3DXVECTOR3(1.0f, -1.0f, 1.0f), BLACK},
		{D3DXVECTOR3(0.0f, 1.0f, 0.0f), RED}
	};

	//Rescale the pyramid
	float scale = LANDSCAPEVIEW_TILE_WIDTH / 2.0f;
	for(int i=0; i<iNumVertices; ++i)
		objVertexArray[i].pos *= scale;

	//Describe our vertex buffer
	D3D10_BUFFER_DESC objBuffDesc;
	objBuffDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	objBuffDesc.Usage = D3D10_USAGE_IMMUTABLE;
	objBuffDesc.CPUAccessFlags = 0;
	objBuffDesc.MiscFlags = 0;
	objBuffDesc.ByteWidth = sizeof(Vertex)*iNumVertices;

	D3D10_SUBRESOURCE_DATA objBuffData;
	objBuffData.pSysMem = objVertexArray;

	//Create our vertex buffer and push it to the landscape view
	ID3D10Buffer *pobjVertexBuffer = NULL;
	HRESULT hr = BattleViewManager::getInstance()->getDevice()->CreateBuffer(&objBuffDesc, &objBuffData, &pobjVertexBuffer);
	DirectUtils::CheckHResultAndTrace(hr, __FILE__, (DWORD)__LINE__, L"Error creating vertex buffer for PudEntityView.");
	objEntityView.setVertexBuffer(pobjVertexBuffer);

	//Build our indices
	int iNumIndices = 18;
	DWORD dwIndexArray[] =
	{
		0, 1, 2,
		1, 3, 2,
		2, 3, 4,
		0, 2, 4,
		1, 0, 4,
		3, 1, 4
	};

	//Describe our index buffer
	D3D10_BUFFER_DESC objIndexBuffDesc;
	objIndexBuffDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	objIndexBuffDesc.Usage = D3D10_USAGE_IMMUTABLE;
	objIndexBuffDesc.CPUAccessFlags = 0;
	objIndexBuffDesc.MiscFlags = 0;
	objIndexBuffDesc.ByteWidth = sizeof(DWORD)*iNumIndices;

	D3D10_SUBRESOURCE_DATA objIndexBuffData;
	objIndexBuffData.pSysMem = dwIndexArray;

	//Create our index buffer and push it to the landscape view
	ID3D10Buffer* pobjIndexBuffer = NULL;
	HR(BattleViewManager::getInstance()->getDevice()->CreateBuffer(&objIndexBuffDesc, &objIndexBuffData, &pobjIndexBuffer));
	objEntityView.setIndexBuffer(pobjIndexBuffer);

	//Set stride & # of indices the landscape view has so it can draw itself properly
	objEntityView.setNumIndices(iNumIndices);
	objEntityView.setStride(sizeof(Vertex));

	//All went well, except possible CreateBuffer calls, though we handle those separately
	return RC_OK;
}

ReturnCode PudEntityView::getWorldMatrix(D3DXMATRIX &mWorld)
{
	//Check for valid state
	if(!m_pobjEntity)
		return RC_ERR_INVALID_STATE;

	//Put together the desired world (x,y,z) coordinates
	GridUnit* pobjGridUnit = m_pobjEntity->getGridUnit();
	if(!pobjGridUnit)
		return RC_ERR_INVALID_STATE;

	float fX = ((float) pobjGridUnit->getX() * LANDSCAPEVIEW_TILE_WIDTH) + (LANDSCAPEVIEW_TILE_WIDTH / 2.0f);
	float fZ = ((float) pobjGridUnit->getZ() * LANDSCAPEVIEW_TILE_WIDTH) + (LANDSCAPEVIEW_TILE_WIDTH / 2.0f);
	float fY = (float) pobjGridUnit->getHeight() * LANDSCAPEVIEW_TILE_HEIGHT + 5.0f;

	//wtf?
	//fX *= -1;
	//fZ *= -1;
	//fY *= -1;

	//Build matrix and return successfully
	DirectUtils::BuildWorldMatrixWithSameOrientation(fX, fY, fZ, mWorld);
	return RC_OK;
}
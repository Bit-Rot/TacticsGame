#include <d3d10.h>
#include "LandscapeView.h"
#include "Landscape.h"
#include "Vertex.h"
#include "BattleViewManager.h"
#include "d3dUtil.h"
#include <Windows.h>

LandscapeView::LandscapeView(Landscape* pobjLandscape)
	: m_pobjLandscape(pobjLandscape), m_pobjVertexBuffer(NULL), 
	m_pobjIndexBuffer(NULL), m_uiNumIndices(0), m_uiStride(0)
{
}

LandscapeView::~LandscapeView()
{
	ReleaseCOM(m_pobjIndexBuffer);
	ReleaseCOM(m_pobjVertexBuffer);
}

ReturnCode LandscapeView::draw()
{
	//Make sure we have a proper handle to our device
	ID3D10Device* pobjDevice = BattleViewManager::getInstance()->getDevice();
	if(!pobjDevice || !m_pobjIndexBuffer || !m_pobjVertexBuffer)
		return RC_ERR_INVALID_STATE;

	//Set up our buffers
	UINT offset = 0;
	UINT stride = m_uiStride;
	pobjDevice->IASetVertexBuffers(0, 1, &m_pobjVertexBuffer, &stride, &offset);
	pobjDevice->IASetIndexBuffer(m_pobjIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Draw!
	pobjDevice->DrawIndexed(m_uiNumIndices, 0, 0);

	return RC_OK;
}

ReturnCode LandscapeView::buildViewForTestLandscape(Landscape &objLandscape, LandscapeView &objLandscapeView)
{
	//Get number of vertices buffers that will be required (Gridunits * 8 vertices)
	//TODO: Update this once we have multiple GridUnits per column
	int iNumVertices = getNumVerticesForTestLandscape(objLandscape);
	Vertex *objVertexArray = new Vertex[iNumVertices];

	//allocate memory for our vertex buffer & generate vertices for each grid unit
	int iCurrentVertex = 0;
	for(int i=0; i < objLandscape.getGridWidth(); ++i)
	{
		for (int j=0; j < objLandscape.getGridLength(); ++j)
		{
			//Fetch grid unit
			GridUnit *pobjGridUnit = NULL;
			objLandscape.getGridUnit(i, j, pobjGridUnit);
			if(!pobjGridUnit)
				return RC_ERR_GENERAL;

			//TODO: Remove me eventually, I'm just for testing purposes
			//Generate a shade of green.
			float red, green, blue;
			red = ((float)(rand() % (int)(RAND_MAX * 0.3)))/((float)RAND_MAX);
			green = ((float)((RAND_MAX*0.7) + rand() % (int)(RAND_MAX * 0.3)))/((float)RAND_MAX);
			blue = ((float)(rand() % (int)(RAND_MAX * 0.3)))/((float)RAND_MAX);
			D3DXCOLOR color(red, green, blue, 1.0f);

			float x = 0.0f, y = 0.0f, z = 0.0f;
			//Generate near-left-top vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getHeight()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = color;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate far-left-top vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getHeight()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = color;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate near-right-top vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getHeight()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = color;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate far-right-top vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getHeight()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = color;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate near-left-bot vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getBase()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = BLACK;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate far-left-bot vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getBase()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = BLACK;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate near-right-bot vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getBase()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = BLACK;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);

			//Generate far-right-bot vertex
			x = (float)i*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			y = (float)pobjGridUnit->getBase()*LANDSCAPEVIEW_TILE_HEIGHT;
			z = (float)j*LANDSCAPEVIEW_TILE_WIDTH + LANDSCAPEVIEW_TILE_WIDTH;
			objVertexArray[iCurrentVertex].color = BLACK;
			objVertexArray[iCurrentVertex++].pos = D3DXVECTOR3(x,y,z);
		}
	}

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
	HR(BattleViewManager::getInstance()->getDevice()->CreateBuffer(&objBuffDesc, &objBuffData, &pobjVertexBuffer));
	objLandscapeView.setVertexBuffer(pobjVertexBuffer);

	//Clean up allocated memory that we don't need any more
	delete[] objVertexArray;

	//Get number of indices that will be required (GridUnits * 6 faces * 2 triangles * 3 indices)
	//TODO: Update this once we have multiple GridUnits per column
	int iNumIndices = getNumIndicesForTestLandscape(objLandscape);
	DWORD *dwIndexArray = new DWORD[iNumIndices];

	//allocate memory for our index buffer & generate indices for for each face
	int iCurrentIndex = 0, iBaseVertex = 0;
	for(int i=0; i<objLandscape.getGridWidth(); ++i)
	{
		for(int j=0; j<objLandscape.getGridLength(); ++j)
		{
			//Determine the base vertex (near-top-left) for the current GridUnit
			iBaseVertex = i*objLandscape.getGridLength()*8 + j*8;

			//Top-bottom-left face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 2;
			dwIndexArray[iCurrentIndex++] = iBaseVertex;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 1;

			//Top-top-right face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 2;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 1;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 3;

			//Front-bottom-left face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 6;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 4;
			dwIndexArray[iCurrentIndex++] = iBaseVertex;

			//Front-top-right face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 6;
			dwIndexArray[iCurrentIndex++] = iBaseVertex;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 2;

			//Left-bottom-left face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 4;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 5;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 1;

			//Left-top-right face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 4;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 1;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 0;

			//Back-bottom-left face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 5;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 7;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 3;

			//Back-top-right face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 5;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 3;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 1;

			//Right-bottom-left face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 7;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 6;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 2;

			//Right-top-right face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 7;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 2;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 3;

			//Bottom-bottom-left face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 5;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 4;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 6;

			//Bottom-top-right face
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 5;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 6;
			dwIndexArray[iCurrentIndex++] = iBaseVertex + 7;
		}
	}

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
	objLandscapeView.setIndexBuffer(pobjIndexBuffer);

	//Clean up allocated memory that we don't need any more
	delete[] dwIndexArray;

	//Set stride & # of indices the landscape view has so it can draw itself properly
	objLandscapeView.setNumIndices(getNumIndicesForTestLandscape(objLandscape));
	objLandscapeView.setStride(sizeof(Vertex));

	//All went well, except possible CreateBuffer calls, though we handle those separately
	return RC_OK;
}

int LandscapeView::getNumIndicesForTestLandscape(const Landscape &objLandscape)
{
	return objLandscape.getGridWidth() * objLandscape.getGridLength() * 36;
}

int LandscapeView::getNumVerticesForTestLandscape(const Landscape &objLandscape)
{
	return objLandscape.getGridWidth() * objLandscape.getGridLength() * 8;
}
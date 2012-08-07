#include "EntityView.h"
#include "d3dUtil.h"
#include <windows.h>

EntityView::EntityView()
	: m_pobjEntity(NULL), m_pobjVertexBuffer(NULL), m_pobjIndexBuffer(NULL)
{}

EntityView::~EntityView()
{
	ReleaseCOM(m_pobjVertexBuffer);
	ReleaseCOM(m_pobjIndexBuffer);
}
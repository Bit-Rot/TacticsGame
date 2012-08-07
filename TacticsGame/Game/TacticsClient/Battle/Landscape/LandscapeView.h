#ifndef LANDSCAPEVIEW_H
#define LANDSCAPEVIEW_H

#include <D3DX10.h>
#include "TacticsErrors.h"

#define LANDSCAPEVIEW_TILE_WIDTH 10.0f
#define LANDSCAPEVIEW_TILE_HEIGHT 5.0f

//Forward-declare dependencies
class Landscape;
class ViewManager;

class LandscapeView
{
public:
	LandscapeView(Landscape* pobjLandscape);
	~LandscapeView();

	ReturnCode draw();

	void setVertexBuffer(ID3D10Buffer* &pobjVertexBuffer){m_pobjVertexBuffer = pobjVertexBuffer;}
	void setIndexBuffer(ID3D10Buffer* &pobjIndexBuffer){m_pobjIndexBuffer = pobjIndexBuffer;}
	void setNumIndices(int iNumIndices){m_uiNumIndices = iNumIndices;}
	void setStride(UINT uiStride){m_uiStride = uiStride;}

	//TODO: Get rid of this awful awful abomination at some point
	static ReturnCode buildViewForTestLandscape(Landscape &objLandscape, LandscapeView &objLandscapeView);

protected:
	//Aggregate references
	Landscape* m_pobjLandscape;

	//Owned references
	ID3D10Buffer *m_pobjVertexBuffer;
	ID3D10Buffer *m_pobjIndexBuffer;

	//Owned properties
	UINT m_uiNumIndices;
	UINT m_uiStride;

private:

	//Utility methods
	static int getNumIndicesForTestLandscape(const Landscape &objLandscape);
	static int getNumVerticesForTestLandscape(const Landscape &objLandscape);
};

#endif //#ifndef LANDSCAPEVIEW_H
#ifndef PUDENTITYVIEW_H
#define PUDENTITYVIEW_H

#include "EntityView.h"
#include "PudEntity/PudEntity.h"

/**
 *	<pre>PudEntityView</pre> is the view component corresponding to the <pre>PudEntity</pre> class.
 */
class PudEntityView : public EntityView
{
public:
	//c'tor and d'tor
	PudEntityView();
	~PudEntityView();

	//Virtual methods
	virtual ReturnCode draw();
	virtual ReturnCode getWorldMatrix(D3DXMATRIX &mWorld);

	//TODO: Get rid of me once model is encapsulated in class
	void setStride(UINT uiStride){m_uiStride = uiStride;}
	void setNumIndices(UINT uiNumIndices){m_uiNumIndices = uiNumIndices;}

	//Awful gross test method that should probably die at some point
	static ReturnCode buildViewForPudEntity(PudEntityView &objPudEntityView);

protected:
	//TODO: Get rid of me once model is encapsulated in class
	UINT m_uiStride;
	UINT m_uiNumIndices;

private:
	PudEntity* getEntity();
};

#endif //#ifndef PUDENTITYVIEW_H
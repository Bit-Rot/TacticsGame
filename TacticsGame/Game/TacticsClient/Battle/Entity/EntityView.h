#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include "Entity.h"
#include "TacticsErrors.h"
#include <d3dx10.h>

/**
 *	The <pre>EntityView</pre> class is the view component of the <pre>Entity</pre> class.  Any view components to 
 *	an <pre>Entity</pre> subclass should similarly subclass <pre>EntityView</pre>.
 */
class EntityView
{
public:
	//c'tor and d'tor
	EntityView();
	virtual ~EntityView();

	//Virtual methods
	virtual ReturnCode draw() = 0;
	virtual void setVertexBuffer(ID3D10Buffer* pobjBuffer){m_pobjVertexBuffer = pobjBuffer;}
	virtual void setIndexBuffer(ID3D10Buffer* pobjBuffer){m_pobjIndexBuffer = pobjBuffer;}
	virtual void setEntity(Entity* pobjEntity){m_pobjEntity = pobjEntity;}

	//Virtual methods
	virtual ReturnCode getWorldMatrix(D3DXMATRIX &mWorld) = 0;

protected:
	//Aggregate properties
	Entity* m_pobjEntity;

	//Owned properties
	ID3D10Buffer* m_pobjVertexBuffer;
	ID3D10Buffer* m_pobjIndexBuffer;

	//TODO: Get rid of this awful awful abomination at some point
	static ReturnCode buildViewForPudEntity(Entity &objLandscape, EntityView &objLandscapeView);

private:
};

#endif //#ifndef ENTITYVIEW_H
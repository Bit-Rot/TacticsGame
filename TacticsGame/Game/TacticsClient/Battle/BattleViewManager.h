#ifndef BATTLEVIEWMANAGER_H
#define BATTLEVIEWMANAGER_H

#include "Camera.h"
#include "ViewManager.h"
#include "LandscapeView.h"
#include "TacticsErrors.h"
#include "EntityView.h"
#include "SyncUtil.h"
#include <d3d10.h>
#include <set>

typedef std::set<EntityView*> typeEntityViewSet;
typedef typeEntityViewSet::iterator typeEntityViewIterator;

class BattleViewManager : public ViewManager
{
public:
	//Singleton stuff
	static BattleViewManager* getInstance();
	static void destroyInstance();

	//Virtual methods
	virtual ReturnCode draw();
	virtual void onResize(int iClientWidth, int iClientHeight);

	//Public methods
	ReturnCode init(ID3D10Device* &pobjDevice, const Camera &pobjCamera);

	ReturnCode addEntityView(EntityView* pobjEntityView);
	ReturnCode removeEntityView(EntityView* pobjEntityView);
	
	//Getters/Setters
	void setLandscapeView(LandscapeView* pobjLandscapeView){m_pobjLandscapeView = pobjLandscapeView;}
	LandscapeView* getLandscapeView() const {return m_pobjLandscapeView;}

protected:
	//Aggregate properties
	const Camera* m_pobjCamera;
	LandscapeView* m_pobjLandscapeView;

	//Owned properties
	ID3D10EffectMatrixVariable *m_pobjWvpVar;
	ID3D10Effect *m_pobjFx;
	ID3D10EffectTechnique *m_pobjFxTech;
	ID3D10InputLayout *m_pobjInputLayout;
	D3DXMATRIX m_mProj;
	std::set<EntityView*> m_pobjEntityViewSet;
	CriticalSectionEx CS;

	//Utility code
	void restoreDefaultStates();
private:
	//Singleton stuff
	BattleViewManager();
	~BattleViewManager();
	static BattleViewManager *m_instance;
};


#endif //#ifndef BATTLEVIEWMANAGER_H
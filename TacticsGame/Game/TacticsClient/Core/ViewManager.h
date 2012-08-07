#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <d3d10.h>
#include "TacticsErrors.h"

class ViewManager
{
public:
	ViewManager();
	virtual ~ViewManager();

	virtual ReturnCode draw() = 0;
	virtual void onResize(int iClientWidth, int iClientHeight) = 0;

	void setDevice(ID3D10Device* pobjDevice){m_pobjDevice = pobjDevice;}
	ID3D10Device* getDevice() const {return m_pobjDevice;}

protected:
	//Aggregate references
	ID3D10Device *m_pobjDevice;
private:
};

#endif GAMEVIEW_H

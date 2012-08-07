#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

class Camera
{
public:
	Camera();
	~Camera();

	void update(float dt);

	void getViewMatrix(D3DXMATRIX &mVp) const {mVp = m_mView;}
	float getFovY() const {return m_fFovY;}
	float getNearPlane() const {return m_fNearPlane;}
	float getFarPlane() const {return m_fFarPlane;}

protected:
	D3DXMATRIX m_mView;
	float m_fX, m_fY, m_fZ;
	float m_fTargetX, m_fTargetY, m_fTargetZ;
	float m_fPhi, m_fTheta;
	float m_fRadius;
	float m_fFovY;
	float m_fNearPlane, m_fFarPlane;

private:

};

#endif //#ifndef CAMERA_H
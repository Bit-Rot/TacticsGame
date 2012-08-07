#include "Camera.h"

Camera::Camera()
	: m_fX(0.0f), m_fY(0.0f), m_fZ(0.0f), m_fTargetX(50.0f), m_fTargetY(0.0f), m_fTargetZ(50.0f),
	m_fPhi(PI/4.0f), m_fTheta(0.0f), m_fRadius(400.0f), m_fFovY(0.25f*PI), m_fNearPlane(1.0f), m_fFarPlane(10000.0f)
{
	D3DXMatrixIdentity(&m_mView);
}

Camera::~Camera()
{}


void Camera::update(float dt)
{
	//TODO: Move this functionality to a Camera Controller class
	// Update angles based on input to orbit camera around box.
	if(GetAsyncKeyState('A') & 0x8000)	m_fTheta -= 2.0f*dt;
	if(GetAsyncKeyState('D') & 0x8000)	m_fTheta += 2.0f*dt;
	if(GetAsyncKeyState('W') & 0x8000)	m_fPhi -= 2.0f*dt;
	if(GetAsyncKeyState('S') & 0x8000)	m_fPhi += 2.0f*dt;
	if(GetAsyncKeyState('Q') & 0x8000)	m_fRadius += 500.0f*dt;
	if(GetAsyncKeyState('E') & 0x8000)	m_fRadius -= 500.0f*dt;
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_fTargetX -= 100.0f*dt*cosf(m_fTheta);
		m_fTargetZ -= 100.0f*dt*sinf(m_fTheta);
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_fTargetX += 100.0f*dt*cosf(m_fTheta);
		m_fTargetZ += 100.0f*dt*sinf(m_fTheta);
	}
	if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_fTargetX -= 100.0f*dt*sinf(m_fTheta);
		m_fTargetZ += 100.0f*dt*cosf(m_fTheta);
	}
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_fTargetX += 100.0f*dt*sinf(m_fTheta);
		m_fTargetZ -= 100.0f*dt*cosf(m_fTheta);
	}

	// Restrict the angle mPhi.
	if(m_fPhi < 0.1f)	m_fPhi = 0.1f;
	if(m_fPhi > PI-0.1f)	m_fPhi = PI-0.1f;
	if(m_fRadius < 0.1f) m_fRadius = 0.1f;
	if(m_fRadius > 10000.0f) m_fRadius = 10000.0f;

	// Convert Spherical to Cartesian coordinates: mPhi measured from +y
	// and mTheta measured counterclockwise from -z.
	// Note, this was somewhat counter intuitive at first, but consider the following:
	// - mPhi is controlled by W/S (vertical), mTheta controlled by A/D (horizontal)
	// - physical location relative to origin on horizontal plan is affected by both phi and theta,
	//	 because as we change the vertical angle, we move inwards towards the origin.
	// - think of x as being dependent upon sin(theta), then modified by sin(phi)
	// - think of z as being dependent upon cos(theta), then modified by sin(phi)
	// - think of y as being dependent upon cos(phi)
	m_fX =  m_fTargetX + m_fRadius*sinf(m_fPhi)*sinf(m_fTheta);
	m_fZ =  m_fTargetZ - m_fRadius*sinf(m_fPhi)*cosf(m_fTheta);
	m_fY =  m_fTargetY + m_fRadius*cosf(m_fPhi);

	//Generate matrices for current position, target, and universal up
	//TODO: Evaluate whether (some of) these should be data members or not
	D3DXVECTOR3 pos(m_fX, m_fY, m_fZ);
	D3DXVECTOR3 target(m_fTargetX, m_fTargetY, m_fTargetZ);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	//Update View-Projection matrix
	D3DXMatrixLookAtLH(&m_mView, &pos, &target, &up);
}
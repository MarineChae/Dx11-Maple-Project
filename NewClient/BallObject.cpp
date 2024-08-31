#include "BallObject.h"
#include"Timer.h"
#include"Collider.h"
extern float MapSizeX;

extern float MapSizeY;
bool BallObject::Frame()
{
	bool chk = SpriteObject::Frame();
    m_fLifeTime += Timer::GetInstance().GetSecPerFrame();
    if (m_fLifeTime >= 20)
        return false;

	TVector3 velo = m_vDir * Timer::GetInstance().GetSecPerFrame() * 300;

	SetTransform(GetTransform() + velo);
	if (GetTransform().x <= -MapSizeX)
	{

		m_vDir.x *= -1.0f;
		SetTransform({ -MapSizeX ,GetTransform().y,GetTransform().z });

	}
	if (GetTransform().y <= -MapSizeY)
	{

		m_vDir.y *= -1.0f;
		SetTransform({ GetTransform().x ,-MapSizeY,GetTransform().z });

	}
	if (GetTransform().x >= MapSizeX)
	{

		m_vDir.x *= -1.0f;
		SetTransform({ MapSizeX ,GetTransform().y,GetTransform().z });

	}
	if (GetTransform().y >= MapSizeY)
	{

		m_vDir.y *= -1.0f;
		SetTransform({ GetTransform().x ,MapSizeY,GetTransform().z });


	}
	GetCollider()->SetTransform(GetTransform());
    return true;
}

BallObject::BallObject()
	:m_fLifeTime(0.0f)
	, m_vDir(1,1,0)
{
}

BallObject::~BallObject()
{
}

#include"Netstd.h"
#include "PlayerData.h"
#include"Collision.h"
#include"Timer.h"
void PlayerData::Init(BOOL	Flag, DWORD SessionID, PLAYER_STATE Action, BYTE Direction, float X, float Y, int HP)
{
	m_bFlag = Flag;
	m_dwSessionID = SessionID;
	m_dwAction = Action;
	m_byDirection = Direction;
	m_vPos = { X,Y,0 };
	m_iHP = HP;
	
}

void PlayerData::Update()
{

	int coefficient = 0;

	//¿ÞÂÊ
	if (m_bisMove)
	{
		if (m_byDirection == 0)
		{
			float t = Timer::GetInstance().GetSecPerFrame();
			m_vMovePow.x = min(m_vMovePow.x, 200);
			m_vMovePow.x -= static_cast<float>(1000 * 0.0625);
			m_vMovePow.x = max(m_vMovePow.x, -500);
		}
		else
		{
			m_vMovePow.x = max(m_vMovePow.x, -200);
			m_vMovePow.x += static_cast<float>(1000 *0.0625);
			m_vMovePow.x = min(m_vMovePow.x, 500);
		}

	}
	else
	{
		m_vMovePow = m_vMovePow.Lerp(m_vMovePow, TVector3::Zero, 0.0625);
	}
	if (m_bIsFalling && !m_bIsJumping)
	{
		m_vPos.y -= static_cast<float>(800 * 0.0625);
	}
	if (m_bIsJumping)
	{
		
		m_vPos.y += static_cast<float>(0.0625 * 800);
		if (m_vPos.y - m_vBeforePos.y > fabs(200.0f))
		{
			m_bIsJumping = false;
			m_bIsFalling = true;
		}
	}

	m_vPos = m_vPos + (m_vMovePow* 0.0625);// (m_vMovePow * Timer::GetInstance().GetSecPerFrame());//TVector3::Lerp(m_vPos, m_vPos + m_vMovePow, Timer::GetInstance().GetSecPerFrame());
}

void PlayerData::SetCollisionPoint()
{
	m_vCollisionPoint = { m_vPos.x,m_vPos.y - m_fHeight,0 };
}



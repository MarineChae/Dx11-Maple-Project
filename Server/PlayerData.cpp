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
	m_bIsFalling = false;
	m_colliderData.Init();
}

void PlayerData::Update()
{
	m_colliderData.SetPos(m_vPos);
	m_colliderData.Update();

	int coefficient = 0;

	//왼쪽
	if (m_bisMove&&!m_bOnLope)
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
		//예상수정위치

	}
	else
	{
		m_vMovePow.x = m_vMovePow.x * (1 - 1) + 0 * 1;
	}
	if (m_bIsFalling && !m_bIsJumping)
	{
		m_vPos.y -= static_cast<float>(700 * 0.0625);
	}
	if (m_bIsJumping)
	{
		if (m_vPos.y - m_vBeforePos.y > fabs(200.0f))
		{
			m_bIsJumping = false;
			m_bIsFalling = true;
		}
		m_vPos.y += static_cast<float>(0.0625 * 700);

	}

	m_vPos = m_vPos + (m_vMovePow* 0.0625);// (m_vMovePow * Timer::GetInstance().GetSecPerFrame());//TVector3::Lerp(m_vPos, m_vPos + m_vMovePow, Timer::GetInstance().GetSecPerFrame());
}

void PlayerData::OnLopeProc()
{
	
	if (m_bOnLope)
	{
		m_bIsJumping = false;

		m_vMovePow.x = 0;

		if (m_byLopeUp == 1)
		{

			m_vPos.y += static_cast<float>(300 * 0.0625);
		}
		else if (m_byLopeUp == 0 )
		{
			m_vPos.y -= static_cast<float>(300 * 0.0625);
		}


	}
}


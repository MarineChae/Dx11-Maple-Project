#include"Netstd.h"
#include "PlayerData.h"

void PlayerData::Init(BOOL	Flag, DWORD SessionID, DWORD Action, BYTE Direction, float X, float Y, int HP)
{
	m_bFlag = Flag;
	m_dwSessionID = SessionID;
	m_dwAction = Action;
	m_byDirection = Direction;
	m_vPos = { X,Y,0 };
	m_iHP = HP;
}

#include"Netstd.h"
#include "PlayerData.h"

void PlayerData::Init(BOOL	Flag, DWORD SessionID, DWORD Action, BYTE Direction, short X, short Y, int HP)
{
	m_bFlag = Flag;
	m_dwSessionID = SessionID;
	m_dwAction = Action;
	m_byDirection = Direction;
	m_shX = X;
	m_shY = Y;
	m_iHP = HP;
}

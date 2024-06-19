#include<Windows.h>
#include "PlayerData.h"

void PlayerData::Init(BOOL	Flag, DWORD SessionID, DWORD Action, BYTE Direction, short X, short Y, int HP)
{
	bFlag = Flag;
	dwSessionID = SessionID;
	dwAction = Action;
	byDirection = Direction;
	shX = X;
	shY = Y;
	iHP = HP;
}

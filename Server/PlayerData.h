#pragma once
#include"Singleton.h"

class PlayerData
{
private:
	BOOL	bFlag;
	DWORD	dwSessionID;

	DWORD	dwAction;
	BYTE	byDirection;

	short	shX;
	short	shY;

	int 	iHP;

public:
	DWORD GetSessionID() const { return dwSessionID; };
	DWORD GetAction() const { return dwAction; };
	BYTE  GetDirection()const { return byDirection; };


	short GetXPos() const { return shX; };
	short GetYPos() const { return shY; };
	int   GetHP() const { return iHP; };


};

class PlayerDataMgr : public Singleton< PlayerDataMgr>
{
	friend class Singleton< PlayerDataMgr>;

private:
	PlayerData m_PlayerList[63];


public:
	PlayerData* GetPlayerList() { return m_PlayerList; };
	PlayerData  GetPlayerData(int SessionNum) const { return m_PlayerList[SessionNum]; };

public:
	PlayerDataMgr()
	{
		memset(m_PlayerList, 0, sizeof(PlayerData) * 63);
	}
	~PlayerDataMgr()
	{

	}
};
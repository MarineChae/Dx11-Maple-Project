#pragma once
#include"Singleton.h"
#include"Protocol.h"
#include<vector>
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
	std::vector<PlayerData*> m_PlayerList;


public:
	std::vector<PlayerData*> GetPlayerList() { return m_PlayerList; };
	PlayerData*  GetPlayerData(int SessionNum) { return m_PlayerList[SessionNum]; };

public:
	PlayerDataMgr()
	{
		m_PlayerList.resize(MAX_USER_SIZE);
	}
	~PlayerDataMgr()
	{

	}
};
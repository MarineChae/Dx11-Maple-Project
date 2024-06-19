#pragma once
#include"Singleton.h"
#include"Protocol.h"
#include<vector>
#include"memory"
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

	void Init(BOOL	Flag, DWORD SessionID, DWORD Action, BYTE Direction, short X, short Y, int HP);

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
	std::vector<std::shared_ptr<PlayerData>> m_PlayerList;


public:
	std::vector<std::shared_ptr<PlayerData>>& GetPlayerList() { return m_PlayerList; };
	std::shared_ptr<PlayerData>  GetPlayerData(int SessionNum) { return m_PlayerList[SessionNum]; };

public:
	PlayerDataMgr()
	{
		m_PlayerList.reserve(MAX_USER_SIZE);
	}
	~PlayerDataMgr()
	{

	}
};
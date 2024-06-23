#pragma once
#include"Singleton.h"

class PlayerData
{
private:
	BOOL	m_bFlag;
	DWORD	m_dwSessionID;
			
	DWORD	m_dwAction;
	BYTE	m_byDirection;

	short	m_shX;
	short	m_shY;
			
	int 	m_iHP;

public:

	void Init(BOOL	Flag, DWORD SessionID, DWORD Action, BYTE Direction, short X, short Y, int HP);

	DWORD GetSessionID() const { return m_dwSessionID; };
	DWORD GetAction() const { return m_dwAction; };
	BYTE  GetDirection()const { return m_byDirection; };


	short GetXPos() const { return m_shX; };
	short GetYPos() const { return m_shY; };
	int   GetHP() const { return m_iHP; };

	void SetXPos(short shX) { m_shX = shX; };
	void SetYPos(short shY) { m_shY = shY; };
	void SetHP(int HP) { m_iHP = HP; };
};

class PlayerDataMgr : public Singleton< PlayerDataMgr>
{
	friend class Singleton< PlayerDataMgr>;

private:
	std::vector<std::shared_ptr<PlayerData>> m_PlayerList;


public:
	void PushPlayerData(int SessionNum, std::shared_ptr<PlayerData>data) { m_PlayerList[SessionNum] = data; };
	std::vector<std::shared_ptr<PlayerData>>& GetPlayerList() { return m_PlayerList; };
	std::shared_ptr<PlayerData>  GetPlayerData(int SessionNum) { return m_PlayerList[SessionNum]; };
	void DeletePlayerData(int SessionNum) { m_PlayerList[SessionNum].reset(); };
public:
	PlayerDataMgr()
	{
		m_PlayerList.resize(MAX_USER_SIZE);
	}
	~PlayerDataMgr()
	{

	}
};
#pragma once
#include"Singleton.h"

enum SceneNum
{
	Lobby,
	BossRoom1,
	BossRoom2,
	BossRoom3,
};


class PlayerData
{
private:
	SceneNum m_PlaceScene;

	BOOL	m_bFlag;
	DWORD	m_dwSessionID;
			
	DWORD	m_dwAction;
	BYTE	m_byDirection;

	TVector3 m_vPos;
			
	int 	m_iHP;

public:

	void Init(BOOL	Flag, DWORD SessionID, DWORD Action, BYTE Direction, float X, float Y, int HP);

	DWORD GetSessionID() const { return m_dwSessionID; };
	DWORD GetAction() const { return m_dwAction; };
	BYTE  GetDirection()const { return m_byDirection; };

	SceneNum GetCurrentScene()const { return m_PlaceScene; }

	TVector3 GetPos() const { return m_vPos; };
	int   GetHP() const { return m_iHP; };

	void SetPos(TVector3 pos) { m_vPos = pos; };
	void SetHP(int HP) { m_iHP = HP; };
	void SetCurrentScene(SceneNum currentScene) { m_PlaceScene = currentScene; };
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
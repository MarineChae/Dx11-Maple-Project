#pragma once
#include"Singleton.h"
#include"ColliderData.h"
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
			
	PLAYER_STATE	m_dwAction;
	BYTE	m_byDirection;

	TVector3 m_vPos;
			
	int 	m_iHP;

private:
	//패킷에 보낼필요없음 "아마도?"
	bool    m_bisMove;
	bool m_bIsFalling;
	bool m_bIsJumping;
	TVector3 m_vCollisionPoint;

	TVector3 m_vMovePow;
	TVector3 m_vBeforePos;
	bool m_bOnLope;
	ColliderData m_colliderData;
	BYTE m_byLopeUp;
public:

	void Init(BOOL	Flag, DWORD SessionID, PLAYER_STATE Action, BYTE Direction, float X, float Y, int HP);
	void Update();
	void OnLopeProc();
	//getter
	DWORD GetSessionID() const { return m_dwSessionID; };
	PLAYER_STATE GetAction() const { return m_dwAction; };
	BYTE  GetDirection()const { return m_byDirection; };

	SceneNum GetCurrentScene()const { return m_PlaceScene; }
	TVector3 GetPos() const { return m_vPos; };
	int   GetHP() const { return m_iHP; };
	TVector3 GetCollisionPoint()const { return m_vCollisionPoint; };
	bool  GetIsFalling()const { return m_bIsFalling; }
	bool  GetIsJumping()const { return m_bIsJumping; }
	bool GetIsMove()const { return m_bisMove; };
	bool GetOnLope()const { return m_bOnLope; };
	ColliderData GetCollisionData()const { return m_colliderData; };
	BYTE GetLopeUp()const { return m_byLopeUp; };
	//setter
	void SetPos(TVector3 pos) { m_vPos = pos; };
	void SetHP(int HP) { m_iHP = HP; };
	void SetCurrentScene(SceneNum currentScene) { m_PlaceScene = currentScene; };
	void SetIsFalling(bool falling) { m_bIsFalling = falling; }
	void SetIsJumping(bool Jump) { m_bIsJumping = Jump; }
	void SetIsMove(bool move) { m_bisMove = move; };
	void SetAction(PLAYER_STATE action)  {  m_dwAction= action; };
	void SetDirection(BYTE dir) {  m_byDirection =dir; };
	void SetBeforePos(TVector3 pos) { m_vBeforePos = pos; }
	void SetOnLope(bool onlope) { m_bOnLope = onlope; }
	void SetLopeUp(BYTE lopeUP) { m_byLopeUp = lopeUP; };
	void SetMovePow(TVector3 pow) { m_vMovePow = pow; };
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
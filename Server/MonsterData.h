#pragma once
#include"ColliderData.h"

class BehaviorTree;
class PlayerData;
struct SpriteData
{
	TVector3 m_vOffset;
	TVector3 m_vScale;
	int iCol = 0;
	int iRow = 0;
	int iMaxImageCount = 0;
	float m_fDelay = 0.0f;
};

class MonsterData
{
private:
	
	int		m_iId;
	BYTE    m_iCurrentScene;
	char	m_csMonsterName[80];
	bool	m_bIsDead;
	float   m_fResponTime;
	float   m_fRemainResponTime;

	DWORD	m_dwAction;
	BYTE	m_byDirection;

	TVector3 m_vPos;
	TVector3 m_vOriginPos;
	TVector3 m_vResponPos;
	int     m_iMaxHp;
	int 	m_iHP;

	bool   m_bfly;
	bool   m_bFalling;
	MONSTER_STATE m_MonsterState;
	ColliderData m_colliderData;
	std::vector<SpriteData>	m_vSpriteData;
	//behaviortree 추가해야함

	std::shared_ptr<BehaviorTree> m_pBehaviorTree;
	std::shared_ptr<PlayerData> m_pTargetPlayer;
public:
	void Create(char* name, int Id, DWORD Action, BYTE Direction, float X, float Y, int HP, BYTE icurrentScene);
	std::shared_ptr<BehaviorTree> CreateTree(std::string treename);
	void Update();

	DWORD GetAction() const { return m_dwAction; };
	BYTE  GetDirection()const { return m_byDirection; };
	char* GetName()  {return m_csMonsterName;}
	TVector3 GetPos() const { return m_vPos; };
	TVector3 GetOriginPos() const { return m_vOriginPos; };
	int   GetMaxHP() const { return m_iMaxHp; };
	int   GetHP() const { return m_iHP; };
	BYTE   GetCurrentScene()const { return m_iCurrentScene; }
	int	  GetId()const { return m_iId; };
	bool  GetIsDead()const { return m_bIsDead; };
	bool  GetIsFly()const { return m_bfly; };
	bool  GetIsFalling()const { return m_bFalling; };
	ColliderData& GetCollisionData() { return m_colliderData; };
	void AddSpriteData(SpriteData& data) { m_vSpriteData.push_back(data); };

	void SetPos(TVector3 pos) { m_vPos = pos; };
	void SetOriginPos(TVector3 pos) { m_vOriginPos = pos; };
	void SetMaxHP(int HP) { m_iMaxHp = HP; };
	void SetHP(int HP) { m_iHP = HP; };
	void SetIsFalling(bool fall) { m_bFalling = fall; };
	void SetAction(DWORD action)  { m_dwAction = action; };
	void SetDirection(BYTE dir) { m_byDirection = dir; };
	void SetIsFly(bool fly) { m_bfly = fly; };
	void SetIsDead(bool dead) { m_bIsDead = dead; };
	void SetMonsterState(MONSTER_STATE state);

	void MoveTo(TVector3 dest,float speed);
	std::shared_ptr<PlayerData> GetTargetPlayer() const {return m_pTargetPlayer;}
	void SetTargetPlayer(std::shared_ptr<PlayerData> player) { m_pTargetPlayer = player; }
	TVector3 GetResponPos() const { return m_vResponPos; };
	MONSTER_STATE GetMonsterState()const { return m_MonsterState; }


public:
	MonsterData();
	MonsterData(std::string treename);
	~MonsterData();

};


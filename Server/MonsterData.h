#pragma once
class BehaviorTree;
class PlayerData;
class MonsterData
{
private:
	
	int		m_iId;
	BYTE    m_iCurrentScene;
	char	m_csMonsterName[80];
	bool	m_bIsDead;
	float   m_fResponTime;

	DWORD	m_dwAction;
	BYTE	m_byDirection;

	TVector3 m_vPos;

	int 	m_iHP;
	//behaviortree 추가해야함

	std::shared_ptr<BehaviorTree> m_pBehaviorTree;
	std::shared_ptr<PlayerData> m_pTargetPlayer;
public:
	void Create(char* name, int Id, DWORD Action, BYTE Direction, float X, float Y, int HP, BYTE icurrentScene);

	void Update();

	DWORD GetAction() const { return m_dwAction; };
	BYTE  GetDirection()const { return m_byDirection; };
	char* GetName()  {return m_csMonsterName;}
	TVector3 GetPos() const { return m_vPos; };
	int   GetHP() const { return m_iHP; };
	BYTE   GetCurrentScene()const { return m_iCurrentScene; }
	int	  GetId()const { return m_iId; };

	void SetPos(TVector3 pos) { m_vPos = pos; };

	void SetHP(int HP) { m_iHP = HP; };

	void SetAction(DWORD action)  { m_dwAction = action; };
	void SetDirection(BYTE dir) { m_byDirection = dir; };

	void MoveTo(TVector3 dest);
	std::shared_ptr<PlayerData> GetTargetPlayer() const {return m_pTargetPlayer;}



public:
	MonsterData();
	~MonsterData();

};


#pragma once
class MonsterData
{
private:
	
	char	m_csMonsterName[80];
	bool	m_bIsDead;
	float   m_fResponTime;

	DWORD	m_dwAction;
	BYTE	m_byDirection;

	short	m_shX;
	short	m_shY;

	int 	m_iHP;
	//behaviortree 추가해야함

public:
	void Create(char* name, DWORD Action, BYTE Direction, short X, short Y, int HP);


	DWORD GetAction() const { return m_dwAction; };
	BYTE  GetDirection()const { return m_byDirection; };
	char* GetName()  {return m_csMonsterName;}
	short GetXPos() const { return m_shX; };
	short GetYPos() const { return m_shY; };
	int   GetHP() const { return m_iHP; };

	void SetXPos(short shX) { m_shX = shX; };
	void SetYPos(short shY) { m_shY = shY; };
	void SetHP(int HP) { m_iHP = HP; };

	void SetAction(DWORD action)  { m_dwAction = action; };
	void SetDirection(BYTE dir) { m_byDirection = dir; };

public:
	MonsterData();
	~MonsterData();

};


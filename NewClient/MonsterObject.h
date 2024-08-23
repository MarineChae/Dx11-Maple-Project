#pragma once
#include"SpriteObject.h"

class MonsterObject :public SpriteObject
{
private:
	std::string	  m_csMonsteName;
	bool          m_IsDead;
	float		  m_fResponTime;
	TVector3	  m_ResponPosision;
	DWORD	      m_dwCurrentAction;
	int		      m_iHP;
	BYTE		  m_CurrentiDirection;
	BYTE		  m_BeforeDirection;
	MONSTER_STATE m_MonsterState;

public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	void AddSpriteData(std::shared_ptr<SpriteData> data, std::wstring textureName);
	std::string GetMonsterName() const { return m_csMonsteName; };
	void SetMonsterName(std::string name) { m_csMonsteName = name; };
	void SetResponPos(TVector3 pos) { m_ResponPosision = pos; };
	virtual TVector3 GetResponPos() const override { return m_ResponPosision; };
	virtual void ChangeMonsterState(MONSTER_STATE state)override;
public:
	virtual bool GetIsDead() { return m_IsDead; }
	virtual void SetIsDead(bool dead) { m_IsDead = dead; };


public:
	MonsterObject();
	virtual ~MonsterObject();




};


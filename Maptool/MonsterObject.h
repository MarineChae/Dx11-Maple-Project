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
	std::string m_treename;

public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	void AddSpriteData(std::shared_ptr<SpriteData> data, std::wstring textureName);

	std::string GetMonsterName() const { return m_csMonsteName; };
	void SetMonsterName(std::string name) { m_csMonsteName = name; };
	void SetTreeName(std::string name) { m_treename = name; };
	std::string GetTreeName() { return m_treename; };
	int GetHP()const { return m_iHP; };
	void SetHP(int hp) {  m_iHP = hp; };
public:
	MonsterObject();
	virtual ~MonsterObject();




};


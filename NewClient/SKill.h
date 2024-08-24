#pragma once
#include"Singleton.h"
#include"SpriteObject.h"

class PlayerData;
class Skill : public SpriteObject
{
private:
	std::string m_csSkillName;
	TVector3    m_vOffset;
	bool		m_bEnable;

	//히트이펙트도
public:
	void SetSkillName(std::string st) { m_csSkillName = st; };
	void SetOffset(TVector3 offset) { m_vOffset = offset; };
	TVector3 GetOffset() const { return m_vOffset; }
	void SetEnable(bool enable) { m_bEnable = enable; };
	bool GetEnable()const { return m_bEnable; };
	std::string GetSkillName() { return m_csSkillName; }
public:
	virtual bool Frame()override;
	virtual bool Render()override;

public:
	Skill();
	virtual ~Skill();

};


////스킬 매니저를 만들어서 몇번 스킬을 사용하는지 가져옴
////

class SkillMgr :public Singleton< SkillMgr>
{
public:

	std::map<std::string, std::shared_ptr<Skill>> m_SkillMap;

public:
	std::map<std::string, std::shared_ptr<Skill>> GetSkillMap() const  { return m_SkillMap; };
	std::shared_ptr<Skill> GetSkill(std::string num);
	std::shared_ptr<Skill> LoadSkill(std::string num);

};
#pragma once
#include"Singleton.h"
#include"SpriteObject.h"
class Sound;
class PlayerData;
class Skill : public SpriteObject
{
private:
	std::string m_csSkillName;
	std::string m_csSkillNum;
	TVector3    m_vOffset;
	bool		m_bEnable;
	bool		m_bCanHit;
	std::shared_ptr<SpriteObject> m_pEffect;
	std::shared_ptr<Sound> m_pSkillSound;
	//히트이펙트도
	std::shared_ptr<Sound> m_pSkillHitSound;
	std::vector<TVector3> m_vHitPos;
	int hitmaxcnt = 8;
	int hitcnt = 0;
	float hitdelay=0.0f;
	bool	m_bPlayEffSound;
public:
	//getter
	TVector3 GetOffset() const { return m_vOffset; }
	bool GetEnable()const { return m_bEnable; };
	std::string GetSkillName() { return m_csSkillName; }
	std::string GetSkillNum() { return m_csSkillNum; }
	bool GetCanHit()const { return m_bCanHit; };
	std::shared_ptr<Sound> GetSkillSound() { return m_pSkillSound; }
	std::shared_ptr<Sound> GetSkillHitSound() { return m_pSkillHitSound; }
	std::shared_ptr<SpriteObject> GetEffect() { return m_pEffect; }
	//setter
	void SetEnable(bool enable) { m_bEnable = enable; };
	void SetSkillName(std::string st) { m_csSkillName = st; };
	void SetSkillNum(std::string st) { m_csSkillNum = st; };
	void SetOffset(TVector3 offset) { m_vOffset = offset; };
	void SetCanHit(bool canHit) {  m_bCanHit = canHit; };
	void SetSkillSound(std::shared_ptr<Sound> sound) { m_pSkillSound = sound; }
	void SetSkillHitSound(std::shared_ptr<Sound> sound) { m_pSkillHitSound = sound; }
	void SetPlayEffSound(bool play);
	void SetEffect(std::shared_ptr<SpriteObject> eff) { m_pEffect = eff; };
	void PushHitPos(TVector3 pos) { m_vHitPos.push_back(pos); };
public:
	void CopySkill(std::shared_ptr<Skill> skill);
	void PlaySkillSound();
	void PlaySkillEffSound();

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
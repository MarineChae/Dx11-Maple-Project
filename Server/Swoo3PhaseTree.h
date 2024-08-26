#pragma once
#include"BehaviorTree.h"
class Swoo3PhaseTree :public BehaviorTree
{

private:

	float m_fSpawnTime;
	float m_f1SkillColldown;
	float m_f2SkillColldown;
public:
	virtual void Init();
	virtual void Update()override;

public:
	virtual ReturnCode ChasePlayer() override;
	virtual ReturnCode AttackPlayer() override;
	virtual ReturnCode Skill1Cooldown()override;
	virtual ReturnCode Skill1()override;
	virtual void DeathEvent() override;


public:

	Swoo3PhaseTree(MonsterData& data);
	virtual ~Swoo3PhaseTree();



};
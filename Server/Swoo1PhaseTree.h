#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;

class Swoo1PhaseTree:public BehaviorTree
{
	float m_fSpawnTime;
public:
	virtual void Init();
	virtual void Update()override;

public:
	virtual ReturnCode AttackPlayer()override;
	virtual void DeathEvent() override;
public:
	Swoo1PhaseTree(MonsterData& data);

	virtual ~Swoo1PhaseTree() {};
};


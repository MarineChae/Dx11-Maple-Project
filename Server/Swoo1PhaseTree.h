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
	void Update();

public:
	virtual ReturnCode AttackPlayer()override;



public:
	Swoo1PhaseTree(MonsterData& data);

	virtual ~Swoo1PhaseTree() {};
};


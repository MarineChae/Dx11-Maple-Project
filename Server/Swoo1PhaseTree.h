#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;
class ObjectData;
class Swoo1PhaseTree:public BehaviorTree
{
	float m_fSpawnTime;
	std::shared_ptr<ObjectData> m_pLazer ;
	std::shared_ptr<ObjectData> m_pLazerCollider1;
	std::shared_ptr<ObjectData> m_pLazerCollider2;
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


#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;
class FlyingMonsterTree :public BehaviorTree
{
private:

public:
	virtual void Init();

public:

	virtual ReturnCode ChasePlayer() override;
	virtual ReturnCode AttackPlayer() override;
	virtual ReturnCode Respon()override;
	virtual void DeathEvent() override;

public:
	FlyingMonsterTree(MonsterData& data);
	virtual ~FlyingMonsterTree();


};


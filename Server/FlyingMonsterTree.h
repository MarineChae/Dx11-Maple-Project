#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;
class FlyingMonsterTree :public BehaviorTree
{


public:
	virtual void Init();

public:

	virtual ReturnCode ChasePlayer() override;
	virtual ReturnCode AttackPlayer() override;
public:
	FlyingMonsterTree(MonsterData& data);
	virtual ~FlyingMonsterTree();


};


#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;
class FlyingMonsterTree :public BehaviorTree
{
private:
	float m_fWaitTime;

public:
	virtual void Init();

public:

	virtual ReturnCode ChasePlayer() override;
	virtual ReturnCode AttackPlayer() override;
	virtual ReturnCode Respon()override;
public:
	FlyingMonsterTree(MonsterData& data);
	virtual ~FlyingMonsterTree();


};


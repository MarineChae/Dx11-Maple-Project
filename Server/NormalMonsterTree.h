#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;
class NormalMonsterTree :public BehaviorTree
{

public:
	virtual void Init();

public:




public:
	NormalMonsterTree(MonsterData& data);
	virtual ~NormalMonsterTree();
};


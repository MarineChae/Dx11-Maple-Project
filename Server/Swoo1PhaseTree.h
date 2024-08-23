#pragma once
#include"BehaviorTree.h"
class PlayerData;
class MonsterData;
enum ReturnCode;

class Swoo1PhaseTree:public BehaviorTree
{
public:
	Swoo1PhaseTree(MonsterData& data)
		:BehaviorTree(data)
	{};
	virtual ~Swoo1PhaseTree() {};
};


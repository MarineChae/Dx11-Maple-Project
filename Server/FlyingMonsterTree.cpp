#include"Netstd.h"
#include "FlyingMonsterTree.h"
#include "BehaviorTreeNode.h"
#include"MonsterData.h"
#include"PlayerData.h"
#include"MakePacket.h"
#include"Packet.h"
#include"IOCPServer.h"
#include"ServerScene.h"
void FlyingMonsterTree::Init()
{

	std::shared_ptr<SequenceNode> testnode = std::make_shared<SequenceNode>(*this);
	SetRootNode(testnode);

	std::shared_ptr<ActionNode> respon = std::make_shared<ActionNode>(*this, &BehaviorTree::Respon);
	testnode->PushChild(respon);

	std::shared_ptr<ActionNode> acttest = std::make_shared<ActionNode>(*this, &BehaviorTree::ChasePlayer);
	testnode->PushChild(acttest);

	std::shared_ptr<ActionNode> attck = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	testnode->PushChild(attck);
	GetMonsterData().SetIsFly(true);
	GetMonsterData().SetIsDead(true);
	SetRespawnTime(7.0f);

}
ReturnCode FlyingMonsterTree::ChasePlayer()
{
	if (TVector3::Distance(GetMonsterData().GetCollisionData().GetPos(), GetMonsterData().GetTargetPlayer()->GetPos()) >= 50)
	{
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_WALK);
		GetMonsterData().MoveTo(GetMonsterData().GetTargetPlayer()->GetPos(),250);
		return ReturnCode::RUNNING;
	}
	else
	{
				
		return ReturnCode::SUCCESS;
	}
	
}
 
ReturnCode FlyingMonsterTree::AttackPlayer()
{
	if (GetWaitTime() >= 1.3f)
	{
		SetWaitTime(0.0f);
		GetMonsterData().GetCollisionData().SetPos(GetMonsterData().GetResponPos());
		GetMonsterData().SetPos(GetMonsterData().GetResponPos());
		GetMonsterData().SetIsDead(true);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_IDLE);

		return ReturnCode::SUCCESS;
	}
	else
	{
		SetWaitTime(GetWaitTime() + 0.0325f);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_ATTACK);
		return ReturnCode::RUNNING;
	}



}

ReturnCode FlyingMonsterTree::Respon()
{
	if (!GetMonsterData().GetIsDead())
	{
		SetWaitTime(GetWaitTime() + 0.0325f);
		if (GetWaitTime() >= 0.5f)
		{
			
			SetWaitTime(0.0f);
			return ReturnCode::SUCCESS;
		}
		else
		{
			GetMonsterData().SetMonsterState(MONSTER_STATE::MS_RESPAWN);
			return ReturnCode::RUNNING;
		}
	}
	else
	{
		return ReturnCode::SUCCESS;
	}

}

void FlyingMonsterTree::DeathEvent()
{
	GetMonsterData().SetMonsterState(MONSTER_STATE::MS_DIE);
	SetDieTime(GetDieTime() + 0.0325f);
	if (GetDieTime() >= 0.8f)
	{
		SetDieTime(0.0f);
		GetMonsterData().GetCollisionData().SetPos(GetMonsterData().GetResponPos());
		GetMonsterData().SetPos(GetMonsterData().GetResponPos());
		GetMonsterData().SetIsDead(true);
	}
}



FlyingMonsterTree::FlyingMonsterTree(MonsterData& data)
	:BehaviorTree(data)
{
}

FlyingMonsterTree::~FlyingMonsterTree()
{
}

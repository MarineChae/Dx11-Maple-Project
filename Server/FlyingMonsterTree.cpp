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

	///////////트리구조///////////
	//// 현재는 플레이어에게 접근이 기본 접근 후 일정거리에 들어가면
	//// 자폭공격 
	////// 정찰 따로 없음 
	/////// 타겟 플레이어는 랜덤으로 정했으면


	std::shared_ptr<SequenceNode> testnode = std::make_shared<SequenceNode>(*this);
	SetRootNode(testnode);

	std::shared_ptr<ActionNode> acttest = std::make_shared<ActionNode>(*this, &BehaviorTree::ChasePlayer);
	testnode->PushChild(acttest);

	std::shared_ptr<ActionNode> attck = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	testnode->PushChild(attck);




}
ReturnCode FlyingMonsterTree::ChasePlayer()
{
	if (TVector3::Distance(GetMonsterData().GetPos(), GetMonsterData().GetTargetPlayer()->GetPos()) >= 20)
	{
		GetMonsterData().MoveTo(GetMonsterData().GetTargetPlayer()->GetPos(),400);
		return ReturnCode::RUNNING;
	}
	else
	{
				
		return ReturnCode::SUCCESS;
	}
	
}

ReturnCode FlyingMonsterTree::AttackPlayer()
{


	GetMonsterData().SetIsDead(true);
	GetMonsterData().SetPos(GetMonsterData().GetResponPos());


	return ReturnCode::SUCCESS;
}



FlyingMonsterTree::FlyingMonsterTree(MonsterData& data)
	:BehaviorTree(data)
{
}

FlyingMonsterTree::~FlyingMonsterTree()
{
}

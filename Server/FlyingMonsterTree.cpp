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

	///////////Ʈ������///////////
	//// ����� �÷��̾�� ������ �⺻ ���� �� �����Ÿ��� ����
	//// �������� 
	////// ���� ���� ���� 
	/////// Ÿ�� �÷��̾�� �������� ��������


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
		GetMonsterData().SetIsDead(true);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_IDLE);
		GetMonsterData().SetPos(GetMonsterData().GetResponPos());
		return ReturnCode::SUCCESS;
	}
	else
	{
		SetWaitTime(GetWaitTime() + 0.0625f);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_ATTACK);
		return ReturnCode::RUNNING;
	}

	/////////degub///////////
	/*std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

	GetMonsterData().GetTargetPlayer()->SetCurrentScene((SceneNum)3);

	SceneChangePacket(SendPack, GetMonsterData().GetTargetPlayer()->GetSessionID(), 3);
	IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[GetMonsterData().GetTargetPlayer()->GetSessionID()].get(), SendPack);
	auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(2);
	auto curScene = ServerSceneMgr::GetInstance().InsertScene(3);

	BeforeScene->DeleteScenePlayer(GetMonsterData().GetTargetPlayer());
	curScene->AddScenePlayer(GetMonsterData().GetTargetPlayer());

	
	IOCPServer::GetInstance().Broadcasting({ SendPack,3 }, SessionMgr::GetInstance().GetUserList()[GetMonsterData().GetTargetPlayer()->GetSessionID()]);
	IOCPServer::GetInstance().Broadcasting({ SendPack,2 }, SessionMgr::GetInstance().GetUserList()[GetMonsterData().GetTargetPlayer()->GetSessionID()]);
*/


}

ReturnCode FlyingMonsterTree::Respon()
{
	if (!GetMonsterData().GetIsDead())
	{
		SetWaitTime(GetWaitTime() + 0.0625f);
		if (GetWaitTime() >= 0.6f)
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
	SetDieTime(GetDieTime() + 0.0625f);
	if (GetDieTime() >= 0.8f)
	{
		SetDieTime(0.0f);
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

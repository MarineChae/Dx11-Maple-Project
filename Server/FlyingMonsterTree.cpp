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

	std::shared_ptr<ActionNode> respon = std::make_shared<ActionNode>(*this, &BehaviorTree::Respon);
	testnode->PushChild(respon);

	std::shared_ptr<ActionNode> acttest = std::make_shared<ActionNode>(*this, &BehaviorTree::ChasePlayer);
	testnode->PushChild(acttest);

	std::shared_ptr<ActionNode> attck = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	testnode->PushChild(attck);




}
ReturnCode FlyingMonsterTree::ChasePlayer()
{
	if (TVector3::Distance(GetMonsterData().GetPos(), GetMonsterData().GetTargetPlayer()->GetPos()) >= 50)
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
	if (m_fWaitTime >= 1.3f)
	{
		m_fWaitTime = 0.0f;
		GetMonsterData().SetIsDead(true);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_IDLE);
		GetMonsterData().SetPos(GetMonsterData().GetResponPos());
		return ReturnCode::SUCCESS;
	}
	else
	{
		m_fWaitTime += 0.0625f;
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
		m_fWaitTime += 0.0625f;
		if (m_fWaitTime >= 0.6f)
		{
			
			m_fWaitTime = 0.0f;
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



FlyingMonsterTree::FlyingMonsterTree(MonsterData& data)
	:BehaviorTree(data)
	, m_fWaitTime(0.0f)
{
}

FlyingMonsterTree::~FlyingMonsterTree()
{
}

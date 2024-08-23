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
	std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

	GetMonsterData().GetTargetPlayer()->SetCurrentScene((SceneNum)3);

	SceneChangePacket(SendPack, GetMonsterData().GetTargetPlayer()->GetSessionID(), 3);
	IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[GetMonsterData().GetTargetPlayer()->GetSessionID()].get(), SendPack);
	auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(2);
	auto curScene = ServerSceneMgr::GetInstance().InsertScene(3);

	BeforeScene->DeleteScenePlayer(GetMonsterData().GetTargetPlayer());
	curScene->AddScenePlayer(GetMonsterData().GetTargetPlayer());

	
	IOCPServer::GetInstance().Broadcasting({ SendPack,3 }, SessionMgr::GetInstance().GetUserList()[GetMonsterData().GetTargetPlayer()->GetSessionID()]);
	IOCPServer::GetInstance().Broadcasting({ SendPack,2 }, SessionMgr::GetInstance().GetUserList()[GetMonsterData().GetTargetPlayer()->GetSessionID()]);


	return ReturnCode::SUCCESS;
}



FlyingMonsterTree::FlyingMonsterTree(MonsterData& data)
	:BehaviorTree(data)
{
}

FlyingMonsterTree::~FlyingMonsterTree()
{
}

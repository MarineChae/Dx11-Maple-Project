#include"Netstd.h"
#include "Swoo3PhaseTree.h"
#include"BehaviorTreeNode.h"
#include"MonsterData.h"
#include "PlayerData.h"
#include"ServerScene.h"
#include"IOCPServer.h"
#include"Packet.h"
#include"MakePacket.h"

void Swoo3PhaseTree::Init()
{
	std::shared_ptr<SelectorNode> root = std::make_shared<SelectorNode>(*this);
	SetRootNode(root);
	
	/// 스킬 쓸 수 있으면 스킬먼저 사용
	std::shared_ptr<SequenceNode> Skill1seq = std::make_shared<SequenceNode>(*this);
	root->PushChild(Skill1seq);
	std::shared_ptr<DecoratorNode> coolDown = std::make_shared<DecoratorNode>(*this, &BehaviorTree::Skill1Cooldown);
	Skill1seq->PushChild(coolDown);
	std::shared_ptr<ActionNode> SKill1 = std::make_shared<ActionNode>(*this, &BehaviorTree::Skill1);
	Skill1seq->PushChild(SKill1);


	//추격및 공격
	std::shared_ptr<SequenceNode> ChaseAndAttack = std::make_shared<SequenceNode>(*this);
	root->PushChild(ChaseAndAttack);
	std::shared_ptr<ActionNode> chase = std::make_shared<ActionNode>(*this, &BehaviorTree::ChasePlayer);
	ChaseAndAttack->PushChild(chase);
	std::shared_ptr<ActionNode> attack = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	ChaseAndAttack->PushChild(attack);


	GetMonsterData().GetCollisionData().SetWidth(94.0f);

	GetMonsterData().GetCollisionData().SetHeight(108.0f);
	GetMonsterData().SetIsDead(false);
	SetRespawnTime(7777.0f);
}

void Swoo3PhaseTree::Update()
{
	m_f1SkillColldown += 0.0625;
	m_f2SkillColldown += 0.0625;
	m_fSpawnTime += 0.0625;
	if (m_fSpawnTime >= 1.5f)
	{
		std::shared_ptr<Packet> pack = std::make_shared<Packet>();

		std::string st = "../resource/InteractionObj/FallObj";
		int ivalue = 2 + (rand() % 3);
		st += std::to_string(ivalue) + "/FallObj" + std::to_string(ivalue) + ".txt";
		char c[80];
		strcpy_s(c, st.c_str());

		float randx = randstep(-1000, 1000);
		SpawnObjectPacket(pack, randx, 700, 0, c, OBJECT_TYPE::FALLING_OBJECT ,GetMonsterData().GetCurrentScene());
		m_fSpawnTime = 0.0f;
		IOCPServer::GetInstance().Broadcasting({ pack, GetMonsterData().GetCurrentScene() });
	}

}

ReturnCode Swoo3PhaseTree::ChasePlayer()
{
	if (TVector3::Distance(GetMonsterData().GetCollisionData().GetPos(), GetMonsterData().GetTargetPlayer()->GetPos()) >= 500)
	{
		GetMonsterData().MoveTo(GetMonsterData().GetTargetPlayer()->GetPos(), 250);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_WALK);

		return ReturnCode::RUNNING;
	}
	else
	{

		return ReturnCode::SUCCESS;
	}
}

ReturnCode Swoo3PhaseTree::AttackPlayer()
{

	if (GetWaitTime() >= 3.5f)
	{
		int num = GetMonsterData().GetCurrentScene();
		auto list = ServerSceneMgr::GetInstance().GetSceneList();
		auto curScene = list.find(num);

		for (auto& player : curScene->second->GetScenePlayerList())
		{
			player->SetIsHit(false);
		}
		SetWaitTime(0.0f);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_IDLE);
		return ReturnCode::SUCCESS;
	}
	else if (GetWaitTime() >= 0.8f && GetWaitTime() <= 1.7f)
	{
		int num = GetMonsterData().GetCurrentScene();
		auto list = ServerSceneMgr::GetInstance().GetSceneList();
		auto curScene = list.find(num);

		for (auto& player : curScene->second->GetScenePlayerList())
		{
			if (GetMonsterData().GetAttackCollisionData().CheckOBBCollision(player->GetCollisionData()) && !player->GetIsHit())
			{
				TVector3 value(3000, 0, 0);
				if (GetMonsterData().GetDirection() == 0)
				{
					value *= -1;
				}
				player->SetMovePow(player->GetMovepow() + value);
				player->SetIsHit(true);
			}

		}
		SetWaitTime(GetWaitTime() + 0.0625f);
		return ReturnCode::RUNNING;
	}
	else
	{
		SetWaitTime(GetWaitTime() + 0.0625f);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_ATTACK);
		return ReturnCode::RUNNING;
	}


	return ReturnCode();
}

ReturnCode Swoo3PhaseTree::Skill1Cooldown()
{

	if (m_f1SkillColldown >= 15.0f)
	{
		m_f1SkillColldown = 0;
		return ReturnCode::SUCCESS;
	}

	return ReturnCode::FAILURE;
}

ReturnCode Swoo3PhaseTree::Skill1()
{
	if (GetWaitTime() >= 2.6f)
	{
		SetWaitTime(0.0f);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_IDLE);
		return ReturnCode::SUCCESS;
	}

	else
	{
		SetWaitTime(GetWaitTime() + 0.0625f);
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_SKILL1);
		return ReturnCode::RUNNING;
	}
}


void Swoo3PhaseTree::DeathEvent()
{
	GetMonsterData().SetMonsterState(MONSTER_STATE::MS_DIE);
	SetDieTime(GetDieTime() + 0.0625f);
	SetRunState(false);
	if (GetDieTime() >= 6.12f)
	{
		SetDieTime(0.0f);
	
		GetMonsterData().SetIsDead(true);
		GetMonsterData().SetIsDead(true);
		int num = GetMonsterData().GetCurrentScene();
		auto list = ServerSceneMgr::GetInstance().GetSceneList();
		auto curScene = list.find(num);
		for (auto& player : curScene->second->GetScenePlayerList())
		{

			std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();
			player->SetCurrentScene((SceneNum)1);

			SceneChangePacket(SendPack, player->GetSessionID(), 1);
			IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[player->GetSessionID()].get(), SendPack);
			auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(4);
			auto curScene = ServerSceneMgr::GetInstance().InsertScene(1);

			BeforeScene->DeleteScenePlayer(player);
			curScene->AddScenePlayer(player);
			IOCPServer::GetInstance().Broadcasting({ SendPack,1 }, SessionMgr::GetInstance().GetUserList()[player->GetSessionID()]);
			IOCPServer::GetInstance().Broadcasting({ SendPack,4 }, SessionMgr::GetInstance().GetUserList()[player->GetSessionID()]);
			int iId = 0;
			for (auto& monster : curScene->GetSceneMonsterList())
			{
				std::shared_ptr<Packet> pack = std::make_shared<Packet>();
				CreateMonster(pack, iId++, monster->GetName(), 0, monster->GetPos().x, monster->GetPos().y, monster->GetMaxHP(), 1);
				IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[player->GetSessionID()].get(), pack);
			}

			SetRunState(false);
		}
	}
}

Swoo3PhaseTree::Swoo3PhaseTree(MonsterData& data)
	:BehaviorTree(data)
	, m_f1SkillColldown(0.0f)
	, m_f2SkillColldown (0.0f)
	,m_fSpawnTime(0.0f)
{
}

Swoo3PhaseTree::~Swoo3PhaseTree()
{
}

#include"Netstd.h"
#include "Swoo1PhaseTree.h"
#include "BehaviorTreeNode.h"
#include "PlayerData.h"
#include "MonsterData.h"
#include"MakePacket.h"
#include"Timer.h"
#include"IOCPServer.h"
#include"ServerScene.h"

void Swoo1PhaseTree::Init()
{
	std::shared_ptr<SequenceNode> testnode = std::make_shared<SequenceNode>(*this);
	SetRootNode(testnode);

	std::shared_ptr<ActionNode> acttest = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	testnode->PushChild(acttest);
	GetMonsterData().SetIsDead(false);
	GetMonsterData().SetIsFly(true);
	SetRespawnTime(7777.0f);
}

void Swoo1PhaseTree::Update()
{
	//오브젝트 생성해서 위치 로테이션 다보내

	m_fSpawnTime += 0.0625;
	if (m_fSpawnTime >= 1.5f)
	{
		std::shared_ptr<Packet> pack = std::make_shared<Packet>();

		std::string st = "../resource/InteractionObj/FallObj";
		int ivalue = rand() % 2;
 		st += std::to_string(ivalue) + "/FallObj" + std::to_string(ivalue) + ".txt";
		char c[80];
		strcpy_s(c, st.c_str());

		float randx = randstep(-1000, 1000);
		SpawnObjectPacket(pack,randx,700,0,c,GetMonsterData().GetCurrentScene());
		m_fSpawnTime = 0.0f;
		IOCPServer::GetInstance().Broadcasting({ pack, GetMonsterData().GetCurrentScene() });
	}
	

}

ReturnCode Swoo1PhaseTree::AttackPlayer()
{
	OutputDebugString(std::to_wstring(GetMonsterData().GetHP()).c_str());
	if (!GetMonsterData().GetIsDead())
	{
		GetMonsterData().SetMonsterState(MONSTER_STATE::MS_IDLE);

		return ReturnCode::RUNNING;
	}
	else
	{

		return ReturnCode::SUCCESS;
	}

}

void Swoo1PhaseTree::DeathEvent()
{
	GetMonsterData().SetMonsterState(MONSTER_STATE::MS_DIE);
	SetDieTime(GetDieTime() +0.0625f);
	SetRunState(false);
	if (GetDieTime() >= 9.72f)
	{
		SetDieTime(0.0f);

		GetMonsterData().SetIsDead(true);
		for (auto& player : ServerSceneMgr::GetInstance().GetSceneList()[2]->GetScenePlayerList())
		{

			std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();
			player->SetCurrentScene((SceneNum)3);

			SceneChangePacket(SendPack, player->GetSessionID(), 3);
			IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[player->GetSessionID()].get(), SendPack);
			auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(2);
			auto curScene = ServerSceneMgr::GetInstance().InsertScene(3);

			BeforeScene->DeleteScenePlayer(player);
			curScene->AddScenePlayer(player);
			IOCPServer::GetInstance().Broadcasting({ SendPack,3 }, SessionMgr::GetInstance().GetUserList()[player->GetSessionID()]);
			IOCPServer::GetInstance().Broadcasting({ SendPack,2 }, SessionMgr::GetInstance().GetUserList()[player->GetSessionID()]);
			int iId = 0;
			for (auto& monster : curScene->GetSceneMonsterList())
			{
				std::shared_ptr<Packet> pack = std::make_shared<Packet>();
				CreateMonster(pack, iId++, monster->GetName(), 0, monster->GetPos().x, monster->GetPos().y, monster->GetMaxHP(), 3);
				IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[player->GetSessionID()].get(), pack);
			}

			SetRunState(false);
		}
	}
}
Swoo1PhaseTree::Swoo1PhaseTree(MonsterData& data)
	:BehaviorTree(data)
	, m_fSpawnTime(0.0f)
{
};

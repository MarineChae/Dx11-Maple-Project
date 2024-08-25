#include"Netstd.h"
#include "Swoo1PhaseTree.h"
#include "BehaviorTreeNode.h"
#include "PlayerData.h"
#include "MonsterData.h"
#include"MakePacket.h"
#include"Timer.h"
#include"IOCPServer.h"

void Swoo1PhaseTree::Init()
{
	std::shared_ptr<SequenceNode> testnode = std::make_shared<SequenceNode>(*this);
	SetRootNode(testnode);

	std::shared_ptr<ActionNode> acttest = std::make_shared<ActionNode>(*this, &BehaviorTree::AttackPlayer);
	testnode->PushChild(acttest);


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
	if (!GetMonsterData().GetIsDead())
	{
		Update();

		return ReturnCode::RUNNING;
	}
	else
	{

		//강제 씬이동
		return ReturnCode::SUCCESS;
	}

}

Swoo1PhaseTree::Swoo1PhaseTree(MonsterData& data)
	:BehaviorTree(data)
	, m_fSpawnTime(0)
{
};

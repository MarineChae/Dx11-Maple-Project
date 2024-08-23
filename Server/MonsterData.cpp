#include"Netstd.h"
#include "MonsterData.h"
#include"Timer.h"
#include"NormalMonsterTree.h"
#include"FlyingMonsterTree.h"
#include"Swoo1PhaseTree.h"
#include"PlayerData.h"
#include"IOCPServer.h"
#include"Packet.h"
#include"MakePacket.h"





void MonsterData::Create(char* name, int Id, DWORD Action, BYTE Direction, float X, float Y, int HP, BYTE icurrentScene)
{
	strcpy_s(m_csMonsterName, name);
	m_iId = Id;
	m_dwAction = Action;
	m_byDirection = Direction;
	m_vPos = { X,Y,0 };
	m_iHP = HP;
	m_iCurrentScene = icurrentScene;
	m_vResponPos = { X,Y,0 };
	m_bIsDead = true;
	m_fResponTime = 5.0f;
}

std::shared_ptr<BehaviorTree> MonsterData::CreateTree(std::string treename)
{

	static std::unordered_map<std::string, std::function<std::shared_ptr<BehaviorTree>(MonsterData& data)>> factoryMap =
	{
		 {"NormalMonsterTree", [this](MonsterData& data) { return std::make_shared<NormalMonsterTree>(data); }},
		 {"FlyingMonsterTree", [this](MonsterData& data) { return std::make_shared<FlyingMonsterTree>(data); }},
		 {"Swoo1PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo1PhaseTree>(data); }}
	};

	auto it = factoryMap.find(treename);

	if (it != factoryMap.end())
	{
		return it->second(*this);
	}
	else
	{
		return std::make_shared<NormalMonsterTree>(*this);
	}

}

void MonsterData::Update()
{
	if (PlayerDataMgr::GetInstance().GetPlayerData(0) != nullptr&&!m_bIsDead)
	{
		m_pTargetPlayer = PlayerDataMgr::GetInstance().GetPlayerData(0);
		m_pBehaviorTree->RunTree();

	}
	if (m_bIsDead)
	{
		m_fRemainResponTime += 0.0625f;
		if (m_fRemainResponTime >= m_fResponTime)
		{
			m_bIsDead = false;
			m_fRemainResponTime =0;
		}

	}

}

void MonsterData::MoveTo(TVector3 dest,float speed)
{
	//몬스터 정보 패킷만들어서 보내라
	TVector3 dir =  dest- m_vPos;
	dir.Normalize();
	if (dir.x < 0)
		m_byDirection = 0;
	else
		m_byDirection = 1;

	auto t =0.0625* speed;
	m_vPos += dir * t;

}

MonsterData::MonsterData()
	: m_iCurrentScene()
	, m_csMonsterName()
	, m_bIsDead(false)
	, m_fResponTime(0.0f)
	, m_fRemainResponTime(0.0f)
	, m_dwAction(0)
	, m_byDirection(0)
	, m_vPos()
	, m_iHP(0)
	, m_pBehaviorTree()
	, m_MonsterState(MONSTER_STATE::MS_IDLE)
{
	//temp 추후 어떤 트리를 할당할지 파일에서 불러올예정
	m_pBehaviorTree = std::make_shared<FlyingMonsterTree>(*this);
	m_pBehaviorTree->Init();
}

MonsterData::MonsterData(std::string treename)
	: m_iCurrentScene()
	, m_csMonsterName()
	, m_bIsDead(false)
	, m_fResponTime(0.0f)
	, m_fRemainResponTime(0.0f)
	, m_dwAction(0)
	, m_byDirection(0)
	, m_vPos()
	, m_iHP(0)
	, m_pBehaviorTree()
	, m_MonsterState(MONSTER_STATE::MS_IDLE)
{
 	m_pBehaviorTree = CreateTree(treename);
	m_pBehaviorTree->Init();
}

MonsterData::~MonsterData()
{
}

#include"Netstd.h"
#include "MonsterData.h"
#include"Timer.h"
#include"NormalMonsterTree.h"
#include"FlyingMonsterTree.h"
#include"Swoo1PhaseTree.h"
#include"Swoo2PhaseTree.h"
#include"Swoo3PhaseTree.h"
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
	m_byBeforeDirection = Direction;
	m_vPos = { X,Y,0 };
	m_iMaxHp = HP;
	m_iHP = m_iMaxHp;
	m_iCurrentScene = icurrentScene;
	m_vResponPos = { X,Y,0 };
	m_fResponTime = 7.0f;
	SetOriginPos(m_vPos);
	m_colliderData.SetPos(m_vPos);
	m_fAttackColliderOffset = { -500,0,0 };
	m_attackColliderData.SetPos(m_vPos + m_fAttackColliderOffset);
	m_attackColliderData.SetWidth(500.0f);
	m_attackColliderData.SetHeight(400.0f);
}

std::shared_ptr<BehaviorTree> MonsterData::CreateTree(std::string treename)
{

	static std::unordered_map<std::string, std::function<std::shared_ptr<BehaviorTree>(MonsterData& data)>> factoryMap =
	{
		 {"NormalMonsterTree", [this](MonsterData& data) { return std::make_shared<NormalMonsterTree>(data); }},
		 {"FlyingMonsterTree", [this](MonsterData& data) { return std::make_shared<FlyingMonsterTree>(data); }},
		 {"Swoo1PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo1PhaseTree>(data); }},
		 {"Swoo2PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo2PhaseTree>(data); }},
		 {"Swoo3PhaseTree", [this](MonsterData& data) { return std::make_shared<Swoo3PhaseTree>(data); }}
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
	//m_colliderData.SetPos(m_vPos);
	m_colliderData.Update();

	if (m_pTargetPlayer != nullptr && MONSTER_STATE::MS_WALK >= m_MonsterState)
	{
		TVector3 dir = m_pTargetPlayer->GetPos() - m_colliderData.GetPos();
		dir.Normalize();
		if (dir.x < 0)
			m_byDirection = 0;
		else
			m_byDirection = 1;
	}


	if (m_byBeforeDirection != m_byDirection)
	{
		m_byBeforeDirection = m_byDirection;
		m_fAttackColliderOffset *= -1;
	}

	m_attackColliderData.SetPos(m_colliderData.GetPos() + m_fAttackColliderOffset);
	if (m_bFalling && !m_bfly)
	{
		//m_vPos.y -= static_cast<float>(900 * 0.0625);
		float y = m_colliderData.GetPos().y - static_cast<float>(900 * 0.0625);
		m_colliderData.SetPos({ m_colliderData.GetPos().x ,y ,m_colliderData.GetPos().z});
		
	}
	if (PlayerDataMgr::GetInstance().GetPlayerData(0) != nullptr&&!m_bIsDead && GetHP() >= 0)
	{
		m_pTargetPlayer = PlayerDataMgr::GetInstance().GetPlayerData(0);
		m_pBehaviorTree->RunTree();

	}
	if (GetHP() <= 0)
	{ 
		m_pBehaviorTree->DeathEvent();
	}
	if (m_bIsDead)
	{
		
		m_fRemainResponTime += 0.0625f;
		if (m_fRemainResponTime >= m_pBehaviorTree->GetRespawnTime())
		{
		
 			m_iHP = m_iMaxHp;
			m_bIsDead = false;
			m_fRemainResponTime =0;
		}

	}
	
}

void MonsterData::SetMonsterState(MONSTER_STATE state)
{
	if (m_MonsterState == state)
		return;

	SetPos(GetPos() - m_vSpriteData[m_MonsterState].m_vOffset);
	SetPos(GetPos() + m_vSpriteData[state].m_vOffset);

	m_MonsterState = state;
}

void MonsterData::MoveTo(TVector3 dest,float speed)
{
	//몬스터 정보 패킷만들어서 보내라
	TVector3 dir =  dest- m_colliderData.GetPos();
	dir.Normalize();
	if (dir.x < 0)
		m_byDirection = 0;
	else
		m_byDirection = 1;

	auto t =0.0625* speed;
	m_colliderData.SetPos(m_colliderData.GetPos() + dir * t);

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
	, m_bfly(false)
{
	//temp 추후 어떤 트리를 할당할지 파일에서 불러올예정
	m_pBehaviorTree = std::make_shared<NormalMonsterTree>(*this);
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
	, m_bfly(false)
{
 	m_pBehaviorTree = CreateTree(treename);
	m_pBehaviorTree->Init();
}

MonsterData::~MonsterData()
{
}

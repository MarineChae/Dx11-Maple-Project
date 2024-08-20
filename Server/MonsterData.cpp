#include"Netstd.h"
#include "MonsterData.h"
#include"Timer.h"
#include"NormalMonsterTree.h"
#include"PlayerData.h"

void MonsterData::Create(char* name, DWORD Action, BYTE Direction, float X, float Y, int HP)
{
	strcpy_s(m_csMonsterName,name);
	m_dwAction = Action;
	m_byDirection = Direction;
	m_vPos = { X,Y,0 };
	m_iHP = HP;
}

void MonsterData::Update()
{
	if (PlayerDataMgr::GetInstance().GetPlayerData(0) != nullptr)
	{

		m_pTargetPlayer = PlayerDataMgr::GetInstance().GetPlayerData(0);
		m_pBehaviorTree->RunTree();
	}

}

void MonsterData::MoveTo(TVector3 dest)
{
	m_vPos = TVector3::Lerp(m_vPos, {0,0,0}, 0.1);

}

MonsterData::MonsterData()
	:m_csMonsterName()
	,m_bIsDead(false)
	, m_fResponTime(0.0f)
	, m_dwAction(0)
	, m_byDirection(0)
	,m_vPos()
	, m_iHP(0)
	, m_pBehaviorTree()
{
	//temp 추후 어떤 트리를 할당할지 파일에서 불러올예정
	m_pBehaviorTree = std::make_shared<NormalMonsterTree>(*this);
	m_pBehaviorTree->Init();
}

MonsterData::~MonsterData()
{
}

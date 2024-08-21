#include"Netstd.h"
#include "MonsterData.h"
#include"Timer.h"
#include"NormalMonsterTree.h"
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
	//���� ���� ��Ŷ���� ������
	TVector3 dir =  dest- m_vPos;
	dir.Normalize();
	auto t = Timer::GetInstance().GetSecPerFrame();
	m_vPos += dir;

}

MonsterData::MonsterData()
	: m_iCurrentScene()
	, m_csMonsterName()
	, m_bIsDead(false)
	, m_fResponTime(0.0f)
	, m_dwAction(0)
	, m_byDirection(0)
	, m_vPos()
	, m_iHP(0)
	, m_pBehaviorTree()
{
	//temp ���� � Ʈ���� �Ҵ����� ���Ͽ��� �ҷ��ÿ���
	m_pBehaviorTree = std::make_shared<NormalMonsterTree>(*this);
	m_pBehaviorTree->Init();
}

MonsterData::~MonsterData()
{
}

#include "MonsterObject.h"
#include"Collider.h"
#include"Timer.h"
#include"DamageIndicator.h"

bool MonsterObject::Init()
{
	SpriteObject::Init();
	for (int i = 0; i < 3; ++i)
	{
		m_pDamageIndicatorList.push_back(std::make_shared<DamageIndicator>());
		m_pDamageIndicatorList[i]->Init();
	}
	m_BeforeDirection = GetDirection();
	return true;
}

bool MonsterObject::Frame()
{
	for (int i = 0; i < m_pDamageIndicatorList.size(); ++i)
	{
		if(m_pDamageIndicatorList[i]->GetIsValid())
			m_pDamageIndicatorList[i]->Frame(GetTransform(), 100000);
	}

	if (m_IsDead)
		return true;
	SpriteObject::Frame();
	
	if (m_bIsHit)
	{
		for (int i = 0; i < m_pDamageIndicatorList.size(); ++i)
		{
			if (!m_pDamageIndicatorList[i]->GetIsValid())
			{
				m_pDamageIndicatorList[i]->SetIsValid(true);
				break;
			}
		}
		m_bIsHit = false; 
	}
	if (m_BeforeDirection != GetDirection())
	{
		m_BeforeDirection = GetDirection();
		for (auto& sprite : GetSpriteList())
		{

			sprite->m_vOffset.x = sprite->m_vOffset.x * -1;
			int a = 0;
		}
		}



	SetTransform(GetCollider()->GetTransform() + GetCurrentSpriteInfo()->m_vOffset); 

	if (GetDestination() != GetCollider()->GetTransform())//&& ObejctMgr::GetInstance().GetPlayerObject().get() != this)
	{
		auto tr = GetCollider()->GetTransform();
		auto des = GetDestination();
		GetCollider()->SetTransform(GetCollider()->GetTransform().Lerp(GetCollider()->GetTransform(), GetDestination(), static_cast<float>(Timer::GetInstance().GetSecPerFrame())));
		GetCollider()->SetTransform(GetCollider()->GetTransform().SmoothStep(GetCollider()->GetTransform(), GetDestination(), 0.1f));

	} 
	
	return true;
} 

bool MonsterObject::Render()
{	
	////�״� Ÿ�̹��� �������� ��������
	// �����̺�Ʈ �޼ҵ带 ����ؼ��״� ����� ���� �� Ŭ���̾�Ʈ���� ��� ó���� ��������ҰͰ���
	// �ٵ� �׷��� ��� Ÿ���� �����ٵ�?
	// �׷� ������Ʈ�� �����϶��� Ÿ�� ���ϰ� ����
	if (m_IsDead)
		return false;
	SpriteObject::Render();
	for (int i = 0; i < m_pDamageIndicatorList.size(); ++i)
	{
		if (m_pDamageIndicatorList[i]->GetIsValid())
			m_pDamageIndicatorList[i]->Render();
	} 
	
	return true;
}

bool MonsterObject::Release()
{
	return true;
}

void MonsterObject::AddSpriteData(std::shared_ptr<SpriteData> data , std::wstring textureName)
{
	Create(textureName, L"../Shader/Defalutshader.hlsl");
}
void MonsterObject::ChangeMonsterState(MONSTER_STATE state)
{
	if (m_MonsterState == state)
	{

		return;
	}
		

	InitTexIndex();

	m_MonsterState = state;
	SetScale(GetSpriteData(state)->m_vScale);
	SetTexture(GetSpriteData(state)->m_pTexture);
	SetSpriteInfo(GetSpriteData(state));



}
MonsterObject::MonsterObject()
	:SpriteObject()
	, m_IsDead(false)
	, m_fResponTime()
	, m_dwCurrentAction()
	, m_iHP()
	, m_CurrentiDirection()
	, m_BeforeDirection()
	, m_MonsterState(MS_IDLE)
	, m_bIsHit(false)
{

}

MonsterObject::~MonsterObject()
{
}

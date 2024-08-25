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

	return true;
}

bool MonsterObject::Frame()
{
	for (int i = 0; i < m_pDamageIndicatorList.size(); ++i)
	{
		if(m_pDamageIndicatorList[i]->GetIsValid())
			m_pDamageIndicatorList[i]->Frame(GetTransform(), 1111111);
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



	if (GetDestination() != GetTransform())//&& ObejctMgr::GetInstance().GetPlayerObject().get() != this)
	{
		auto tr = GetTransform();
		auto des = GetDestination();
		SetTransform(GetTransform().Lerp(GetTransform(), GetDestination(), static_cast<float>(Timer::GetInstance().GetSecPerFrame())));
		SetTransform(GetTransform().SmoothStep(GetTransform(), GetDestination(), 0.1f));

	}
	GetCollider()->SetTransform(GetTransform());
	return true;
}

bool MonsterObject::Render()
{	
	if (m_IsDead)
	{
		for (int i = 0; i < m_pDamageIndicatorList.size(); ++i)
		{
			if (m_pDamageIndicatorList[i]->GetIsValid())
				m_pDamageIndicatorList[i]->Render();
		}
	}
	else
	{
		SpriteObject::Render();
		for (int i = 0; i < m_pDamageIndicatorList.size(); ++i)
		{
			if (m_pDamageIndicatorList[i]->GetIsValid())
				m_pDamageIndicatorList[i]->Render();
		}
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
		return;

	m_MonsterState = state;
	InitTexIndex();
	SetSpriteInfo(GetSpriteData(state));
	SetScale(GetCurrentSpriteInfo()->m_vScale);
	SetTexture(GetCurrentSpriteInfo()->m_pTexture);
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

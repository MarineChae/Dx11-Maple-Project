#include "MonsterObject.h"
#include"Collider.h"
#include"Timer.h"

bool MonsterObject::Init()
{
	SpriteObject::Init();
	return true;
}

bool MonsterObject::Frame()
{
	if (m_IsDead)
		return true;
	SpriteObject::Frame();
	

	if (GetDestination() != GetTransform())//&& ObejctMgr::GetInstance().GetPlayerObject().get() != this)
	{
		auto tr = GetTransform();
		auto des = GetDestination();
		SetTransform(GetTransform().Lerp(GetTransform(), GetDestination(), static_cast<float>(Timer::GetInstance().GetSecPerFrame())));
		SetTransform(GetTransform().SmoothStep(GetTransform(), GetDestination(), 0.08f));

	}
	GetCollider()->SetTransform(GetTransform());
	return true;
}

bool MonsterObject::Render()
{
	if (m_IsDead)
		return true;
	SpriteObject::Render();
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

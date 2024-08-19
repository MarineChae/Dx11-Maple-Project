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
	SpriteObject::Frame();
	
	////debug
	static float testtime;
	testtime += Timer::GetInstance().GetSecPerFrame();
	static int size = GetSpriteList().size();
	static int num=0;
	if (testtime >= 2.0)
	{
		testtime = 0;
		if (num >= size)
			num = 0;
		m_MonsterState = (MONSTER_STATE)num++;
		InitTexIndex();
		auto data = GetSpriteData(m_MonsterState);
		SetSpriteInfo(data);
		auto tete = GetCurrentSpriteInfo();
		SetScale(tete->m_vScale);
		SetTexture(tete->m_pTexture);
	}

	GetCollider()->Frame();

	return true;
}

bool MonsterObject::Render()
{
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

MonsterObject::MonsterObject()
	:SpriteObject()
	, m_IsDead()
	, m_fResponTime()
	, m_dwCurrentAction()
	, m_iHP()
	, m_CurrentiDirection()
	, m_BeforeDirection()
	, m_MonsterState(MS_IDLE)
{

}

MonsterObject::~MonsterObject()
{
}

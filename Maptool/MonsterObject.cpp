#include "MonsterObject.h"
#include"Collider.h"
bool MonsterObject::Init()
{
	SpriteObject::Init();
	return true;
}

bool MonsterObject::Frame()
{
	SpriteObject::Frame();
	
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

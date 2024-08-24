#include "Scene.h"
#include"Camera.h"
#include"Collider.h"
void Scene::ResetMap(std::wstring MapName)
{
	m_pMap.reset();
	m_pCollider.reset();
	m_LineColliderList.clear();
	m_PlayerList.clear();
	m_MonsterList.clear();
	m_ObjectList.clear();
	m_PotalList.clear();
	m_LineColliderList.clear();
	Init(MapName);
}
bool Scene::Init(std::wstring MapName)
{
	m_pMap = std::make_shared<Object>();
	std::size_t found = MapName.find_last_of(L"\\");
	std::wstring path = MapName.substr(0, found + 1);
	std::wstring Key = MapName.substr(found + 1);
	m_sMapName = Key;
	found = Key.find_last_of(L".");
	m_sMapName = Key.substr(0, found);


	m_pMap->Create(MapName,L"../Shader/Defalutshader.hlsl");
	m_pCollider = std::make_shared<Collider>();
	m_pCollider->Create(L" ", L"../Shader/LineDebug.hlsl");

    return true;
}

bool Scene::Frame()
{
	m_pMap->Frame();
	for (auto& ob : m_ObjectList)
	{
		ob->Frame();
		ob->GetCollider()->Frame();
	}

	for (auto& m : m_MonsterList)
	{
		m->Frame();
		m->GetCollider()->Frame();
	}
	m_pCollider->Frame();

    return true;
}

bool Scene::Render()
{
	m_pMap->SetMatrix(nullptr,&CameraMgr::GetInstance().GetCamera().GetView(),
							  &CameraMgr::GetInstance().GetCamera().GetProjection());
	m_pMap->Render();

	for (auto& ob : m_ObjectList)
	{
		ob->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
			&CameraMgr::GetInstance().GetCamera().GetProjection());
		ob->Render();
	}
	for (auto& m : m_MonsterList)
	{
		m->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
			&CameraMgr::GetInstance().GetCamera().GetProjection());
		m->Render();
		m->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
			&CameraMgr::GetInstance().GetCamera().GetProjection());
		m->GetCollider()->Render();
	}

	//디버그용
	//m_pCollider->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
	//								&CameraMgr::GetInstance().GetCamera().GetProjection());
	//m_pCollider->Render();

    return true;
}

bool Scene::Release()
{
    return true;
}

Scene::Scene()
	: m_pMap()
	, m_pBackGrondMap(std::make_shared<Object>())
	, m_PlayerList()
	, m_MonsterList()
	, m_LineColliderList()
{

}

Scene::~Scene()
{
	m_PlayerList.clear();
	m_MonsterList.clear();
	m_LineColliderList.clear();
}

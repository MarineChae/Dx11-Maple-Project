#include "Scene.h"
#include"Camera.h"
#include"Collider.h"
#include"PlayerObject.h"
#include"UI.h"
#include"SaveLoader.h"
#include"SoundMgr.h"
#include"Timer.h"
#include"EffectSpwaner.h"
std::shared_ptr<SpriteObject> LaserEff = std::make_shared<SpriteObject>();
std::shared_ptr<SpriteData> LaserEffdata = std::make_shared<SpriteData>();
extern float MapSizeX;
extern float MapSizeY;

void Scene::DeletePlayer(std::shared_ptr<PlayerObject> object)
{
	for (auto it = m_PlayerList.begin(); it != m_PlayerList.end(); )
	{
		if (*it == object)
		{
			it = m_PlayerList.erase(it);
			break;
		}
		else
			++it;
	}

}
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
	//m_pBGM = SoundMgr::GetInstance().Load(L"../resource/Sound/BigMachine_mission.mp3");
	//m_pBGM->SoundPlay(true);
	
	LaserEffdata->iCol = 6;
	LaserEffdata->iMaxImageCount = 6;
	LaserEffdata->iRow = 1;
	LaserEffdata->m_fDelay = 0.10;
	LaserEffdata->m_vScale = { 188,168,1 };

	LaserEff->SetSpriteInfo(LaserEffdata);
	LaserEff->Create(L"../resource/Skill/laserHit.png",L"../Shader/Defalutshader.hlsl");
	LaserEff->SetScale({188,168,1});
    return true;
}

bool Scene::Frame()
{
	if (SceneMgr::GetInstance().GetCurrentScene()->GetSceneNum() > 1)
	{
		if (!m_MonsterList.empty())
		{
			///hp ui 값
			float HPinterval = static_cast<float> (m_MonsterList[0]->GetHp()) / m_MonsterList[0]->GetMaxHp();
			auto hpBar = UIMgr::GetInstance().GetUI()->GetUIObject(UI_DESC::BOSS_HP);
			hpBar->SetScale({ 1015 * HPinterval,13,1 });
			float movebar = (1015 * HPinterval);
			hpBar->SetTransform({ -1030 + movebar , 715.0f, 1 });
		}
			
	}

	m_pMap->Frame();
	for (auto it = m_ObjectList.begin(); it != m_ObjectList.end();)
	{
		auto& ob = *it;
		if (ob->GetObjectType() == ObejctType::LAZER_OBJECT )
		{
			ob->Frame();
			ob->GetCollider()->Frame();
			ob->m_vRotate.z += Timer::GetInstance().GetSecPerFrame() * 0.33f;
		}
		if (ob->GetObjectType() == ObejctType::BALL_OBJECT)
		{
			ob->GetCollider()->Frame();
			if (!ob->Frame())
			{
				it = m_ObjectList.erase(it);
				continue;
			}
			for (auto& player : ObejctMgr::GetInstance().GetObjectList())
			{
				if (player == nullptr) continue;
				if (Collider::CheckOBBCollision(ob->GetCollider(), player->GetCollider()) && !player->GetIsHit())
				{
					EffectSpwaner::GetInstance().SpawnEffect(LaserEff, player->GetTransform(), 0);
					player->SetHp(player->GetHp() - 15000);
					player->SetIsHit(true);
				}
			}
		}
		if (ob->GetObjectType() == ObejctType::COLLIDER && m_MonsterList[0]->GetHp()>=0)
		{
			ob->Frame();
			ob->GetCollider()->Frame();
			ob->GetCollider()->m_vRotate.z += Timer::GetInstance().GetSecPerFrame() * 0.33f;
			for (auto& player : ObejctMgr::GetInstance().GetObjectList())
			{
				if (player == nullptr) continue;
				if (Collider::CheckOBBCollision(ob->GetCollider(), player->GetCollider()) && !player->GetIsHit())
				{
					EffectSpwaner::GetInstance().SpawnEffect(LaserEff, player->GetTransform(),0);
					player->SetHp(player->GetHp() - (player->GetMaxHp() / 2));
					player->SetIsHit(true);
				}
			}
		}
		else
		{
			ob->Frame();
			ob->GetCollider()->Frame();
		}
		++it;
	}

	for (auto& m : m_MonsterList)
	{
		m->Frame();
		m->GetCollider()->Frame();
	}

 	for (auto it = m_InteractObjectList.begin(); it != m_InteractObjectList.end();)
	{
		if (!it->get()->Frame())
		{
			it = m_InteractObjectList.erase(it);
		}
		else
		{

			it->get()->GetCollider()->Frame();
		
			++it;
		}
		
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
		if (ob->GetObjectType() == ObejctType::LAZER_OBJECT && m_MonsterList[0]->GetHp() >= 0)
		{
			ob->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
				&CameraMgr::GetInstance().GetCamera().GetProjection());
			ob->Render();
		}
		else if (ob->GetObjectType() != ObejctType::LAZER_OBJECT)
		{
			ob->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
				&CameraMgr::GetInstance().GetCamera().GetProjection());
			ob->Render();
		}

		if (ob->GetObjectType() == ObejctType::COLLIDER)
		{
			ob->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
				&CameraMgr::GetInstance().GetCamera().GetProjection());
			ob->GetCollider()->Render();
		}
	}
	for (auto& m : m_MonsterList)
	{
		m->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
			&CameraMgr::GetInstance().GetCamera().GetProjection());
		m->Render();
		//m->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
		//	&CameraMgr::GetInstance().GetCamera().GetProjection());
		//m->GetCollider()->Render();
	}
	for (auto& ob : m_InteractObjectList)
	{

			ob->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
				&CameraMgr::GetInstance().GetCamera().GetProjection());
			ob->Render();


		if (ob->GetCollider() != nullptr)
		{
			ob->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(),
				&CameraMgr::GetInstance().GetCamera().GetProjection());
			ob->GetCollider()->Render();
		}
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

std::shared_ptr<Scene> SceneMgr::GetScene(int key)
{
	auto it = m_mSceneList.find(key);

	if (it == m_mSceneList.end())
	{
		std::string st = "../resource/MapObejct/";
		st += std::to_string(key);
		st += ".txt";
		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		SaveLoadMgr::GetInstance().GetSaveLoader().LoadData(scene, st);
		m_mSceneList.insert({ key,scene });
		return scene;
	}

	return it->second;
}

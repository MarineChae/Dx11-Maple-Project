#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
#include "Packet.h"
#include "MakePacket.h"
#include "ClientNet.h"
#include"Collider.h"
#include"Collision.h"
#include"Scene.h"
#include"SaveLoader.h"
#include"Texture.h"
bool ClientMain::Init()
{

	//test = std::make_shared<Object>();	
	//test->SetTransform(TVector3(0, -600, 0));
	//test->SetScale(TVector3(1800, 64, 0));
	//test->Init();
	//test->Create(L"../resource/MapObejct/2Phase BackCollision.png", L"../Shader/Defalutshader.hlsl");
	//test->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
	//test->GetCollider()->SetTransform(test->GetTransform());
	//test->GetCollider()->SetScale(TVector3(1800, 64, 0));
	//test->GetCollider()->SetCollsionType(COLLISION_TYPE::CT_FLOOR);
	//test->m_vRotate.z = -45;
	//test->GetCollider()->m_vRotate.z = -45;

	//ObejctMgr::GetInstance().PushObject(, 60);
	saveload = std::make_shared<SaveLoader>();
	testScene = std::make_shared<Scene>();

	testScene->Init(L"../resource/MapObejct/Lobby.png");


	saveload->LoadData(testScene,"../resource/MapObejct/Lobby.txt");
	MapSizeX = testScene->GetMap()->GetTexture()->GetWidth();
	MapSizeY = testScene->GetMap()->GetTexture()->GetHeight();
	return true;
}

bool ClientMain::Frame()
{
	testScene->Frame();



	if (Input::GetInstance().GetKeyState(VK_F1) >= KEY_PUSH)
	{
		ObejctMgr::GetInstance().GetPlayerObject()->SetTransform({ 0,100,0 });
		ObejctMgr::GetInstance().GetPlayerObject()->SetCurrentScene(SceneNum::BossRoom1);
		testScene->ResetMap(L"../resource/MapObejct/Phase1.png");
		saveload->LoadData(testScene, "../resource/MapObejct/Phase1.txt");
		MapSizeX = testScene->GetMap()->GetTexture()->GetWidth();
		MapSizeY = testScene->GetMap()->GetTexture()->GetHeight();
		CameraMgr::GetInstance().GetCamera().SetZoomScale(1.0f);
		Packet* pack = new Packet;
		
		SceneChangePacket(pack, ObejctMgr::GetInstance().GetPlayerObject()->GetObejctID(),SceneNum::BossRoom1);
		NetSendPacket(pack);


	}
	CameraMgr::GetInstance().GetCamera().SetCameraPos(ObejctMgr::GetInstance().GetPlayerObject()->GetTransform());
	CameraMgr::GetInstance().GetCamera().ControlAngle(1388, 766
													, MapSizeX* CameraMgr::GetInstance().GetCamera().GetZoomScale()
													, MapSizeY * CameraMgr::GetInstance().GetCamera().GetZoomScale());

	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		if (obj != nullptr  && obj->GetCurrentScene() == ObejctMgr::GetInstance().GetPlayerObject()->GetCurrentScene())
		{
			if (obj == ObejctMgr::GetInstance().GetPlayerObject())
			{
				for (auto& line : testScene->GetLineColliderList())
				{
					if (Collision::PointToLine(obj->GetCollider()->GetCollisionPoint(), line) && !obj->GetJumping())
					{
						auto ret = Collision::ClosestPoint(obj->GetCollider()->GetCollisionPoint(), line);
						auto p = obj->GetTransform();
						p.y = ret.y + obj->GetCollider()->GetHeight();
						obj->SetTransform(p);
						obj->SetFalling(false);

						break;
					}
					obj->SetFalling(true);
				}
			}
			obj->Frame();
		}
	}


	

	return true;
}

bool ClientMain::Render()
{
	
	testScene->Render();
	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		if (obj != nullptr && obj->GetCurrentScene() == ObejctMgr::GetInstance().GetPlayerObject()->GetCurrentScene())
		{
			obj->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
			obj->Render();
			if (obj->GetCollider() != nullptr)
			{
				obj->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
				obj->GetCollider()->Render();
			}
		}
		
	}

  	return true;
}

bool ClientMain::Release()
{
	//networkClean();
	return true;
}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);
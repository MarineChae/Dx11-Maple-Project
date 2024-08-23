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
	saveload = std::make_shared<SaveLoader>();
	testScene = std::make_shared<Scene>();
	saveload->LoadData(testScene,"../resource/MapObejct/1.txt");
	MapSizeX = testScene->GetMap()->GetTexture()->GetWidth();
	MapSizeY = testScene->GetMap()->GetTexture()->GetHeight();

	SceneMgr::GetInstance().SetCurrentScene(testScene);

	return true;
}

bool ClientMain::Frame()
{
	testScene->Frame();


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
				bool collision = false;
				for (auto& line : testScene->GetLineColliderList())
				{
					if (obj->GetOnLope())
					{
						obj->SetFalling(false);
					}
					if (line->type == COLLISION_TYPE::CT_WALL)
					{
						if (Collision::isLineIntersectingOBB(line, obj->GetCollider(), 0))
						{
							
						}
					}
					if (line->type == COLLISION_TYPE::CT_LOPE)
					{

						float t = obj->GetCollider()->GetCollisionPoint().y;

						if (Collision::isLineIntersectingOBB(line, obj->GetCollider(), 0))
						{
							collision = true;
							float tempmax = max(line->To.y, line->From.y);
							if (t < tempmax)
							{
								if (Input::GetInstance().GetKeyState('W') >= KEY_PUSH)
								{
							
									obj->SetFalling(false);
									obj->SetOnLope(true);
								}
							}

						}
						if (Collision::isLineIntersectingOBB(line, obj->GetCollider(), 30.f))
						{
							collision = true;
							float tempmin = min(line->To.y, line->From.y);
							if (t >= tempmin)
							{
								if (Input::GetInstance().GetKeyState('S') >= KEY_PUSH)
								{
							
									obj->SetFalling(false);
									obj->SetOnLope(true);
								}
							}
							
						}

					}
					if (!collision)
					{
						obj->SetFalling(true);
					}
					if (line->type == COLLISION_TYPE::CT_FLOOR)
					{
						if (Collision::PointToLine(obj->GetCollider()->GetCollisionPoint(), line))
						{
							obj->SetFalling(false);
							obj->SetOnLope(false);
							collision = true;
						}
					}


				}


				for (auto& potal : testScene->GetPotalList())
				{
					if (Collider::CheckOBBCollision(potal->GetCollider(), obj->GetCollider())
						&& (Input::GetInstance().GetKeyState(VK_UP) >= KEY_PUSH))
					{
						std::string st = "../resource/MapObejct/";
						st += std::to_string(potal->GetNextSceneNum());
						st += ".txt";
						saveload->LoadData(testScene, st);
						MapSizeX = testScene->GetMap()->GetTexture()->GetWidth();
						MapSizeY = testScene->GetMap()->GetTexture()->GetHeight();
						obj->SetTransform({0,0,0});
						obj->SetCurrentScene(potal->GetNextSceneNum());
						CameraMgr::GetInstance().GetCamera().SetZoomScale(1.0f);
						std::shared_ptr<Packet> pack = std::make_shared<Packet>();
						SceneChangePacket(pack, ObejctMgr::GetInstance().GetPlayerObject()->GetObejctID(), potal->GetNextSceneNum());
						NetSendPacket(pack);
					}
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
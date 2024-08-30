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
#include"UI.h"
#include"SoundMgr.h"
#include"EffectSpwaner.h"
extern float MapSizeX = 0;
extern float MapSizeY = 0;
bool ClientMain::Init()
{
	saveload = std::make_shared<SaveLoader>();

	UIMgr::GetInstance().Init();
	return true;
}

bool ClientMain::Frame()
{


	SceneMgr::GetInstance().GetCurrentScene()->Frame();
	EffectSpwaner::GetInstance().Frame();

	CameraMgr::GetInstance().GetCamera().SetCameraPos(ObejctMgr::GetInstance().GetPlayerObject()->GetTransform());
	CameraMgr::GetInstance().GetCamera().ControlAngle(1388, 766
													, MapSizeX
													, MapSizeY);

	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		if (obj != nullptr  && obj->GetCurrentScene() == ObejctMgr::GetInstance().GetPlayerObject()->GetCurrentScene())
		{
			if (obj == ObejctMgr::GetInstance().GetPlayerObject())
			{
				bool collision = false;
				for (auto& line : SceneMgr::GetInstance().GetCurrentScene()->GetLineColliderList())
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
								if (Input::GetInstance().GetKeyState(VK_UP) >= KEY_PUSH)
								{
							
									//obj->SetFalling(false);
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
								if (Input::GetInstance().GetKeyState(VK_DOWN) >= KEY_PUSH)
								{
							
									//obj->SetFalling(false);
									obj->SetOnLope(true);
								}
							}
							
						}

					}
					if (!collision)
					{
						//obj->SetFalling(true);
					}
					if (line->type == COLLISION_TYPE::CT_FLOOR || line->type == COLLISION_TYPE::CT_FINALFLOOR)
					{
						if (Collision::PointToLine(obj->GetCollider()->GetCollisionPoint(), line))
						{
							//obj->SetFalling(false);
							obj->SetOnLope(false);
							collision = true;
						}
					}


				}


				for (auto& potal : SceneMgr::GetInstance().GetCurrentScene()->GetPotalList())
				{
					if (Collider::CheckOBBCollision(potal->GetCollider(), obj->GetCollider())
						&& (Input::GetInstance().GetKeyState(VK_UP) == KEY_PUSH))
					{
						std::shared_ptr<Packet> pack = std::make_shared<Packet>();
						SceneChangePacket(pack, ObejctMgr::GetInstance().GetPlayerObject()->GetObejctID(), potal->GetNextSceneNum());
						NetSendPacket(pack);
					}
				}
			}
			obj->Frame();
		}
	}



	UIMgr::GetInstance().GetUI()->Frame();
	return true;
}

bool ClientMain::Render()
{
	
	SceneMgr::GetInstance().GetCurrentScene()->Render();
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


	EffectSpwaner::GetInstance().Render();
	UIMgr::GetInstance().GetUI()->Render();

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
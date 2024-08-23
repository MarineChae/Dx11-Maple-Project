#include"Netstd.h"
#include"PlayerData.h"
#include"MonsterData.h"
#include "ServerScene.h"
#include"MakePacket.h"
#include"IOCPServer.h"
#include"Timer.h"
#include"Packet.h"
#include"Collision.h"
std::mutex sceneMutex;

void ServerScene::AddScenePlayer(std::shared_ptr<PlayerData> data)
{
	std::lock_guard<std::mutex> lock(sceneMutex);
	{ m_vScenePlayerList.push_back(data); }
}

void ServerScene::DeleteScenePlayer(std::shared_ptr<PlayerData> data)
{
	std::lock_guard<std::mutex> lock(sceneMutex);
	m_vScenePlayerList.erase(std::remove(m_vScenePlayerList.begin(), m_vScenePlayerList.end(), data), m_vScenePlayerList.end());
}

void ServerScene::LoadSceneData(int Scenenum)
{
	FILE* fpRead = nullptr;
	std::string LoadPath="../resource/MapObejct/";
	LoadPath += std::to_string(Scenenum);
	LoadPath += ".txt";

	if (fopen_s(&fpRead, LoadPath.c_str(), "rt") == 0)
	{

		TCHAR buffer[256] = { 0, };

		while (_fgetts(buffer, _countof(buffer), fpRead) != 0)
		{
			TCHAR type[36] = { 0, };

			_stscanf_s(buffer, _T("%s"), type, (unsigned int)_countof(type));

			if (_tcscmp(type, L"#MonsterList") == 0)
			{
				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);
				for (int i = 0; i < iSize; i++)
				{
					TCHAR tex[80] = { 0, };
					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%s\n"), tex, (unsigned int)_countof(tex));
					std::shared_ptr<MonsterData> Monster = std::make_shared<MonsterData>();

					float x;
					float y;
 					_fgetts(buffer, _countof(buffer), fpRead);
					_stscanf_s(buffer, _T("%f %f\n"), &x, &y);
					char name[80];
					WideCharToMultiByte(CP_ACP, 0, tex, sizeof(tex), name, sizeof(name), NULL, NULL);


					Monster->Create(name, i, 0, 0, x, y, 100, Scenenum);

					m_vSceneMonsterList.push_back(Monster);
				}
			}
			else if (_tcscmp(type, L"#LineCollider") == 0)
			{

				_fgetts(buffer, _countof(buffer), fpRead);
				int iSize = 0;
				_stscanf_s(buffer, _T("%d"), &iSize);
				for (int i = 0; i < iSize; i++)
				{
					_fgetts(buffer, _countof(buffer), fpRead);
					std::shared_ptr<Line> line = std::make_shared<Line>();
					_stscanf_s(buffer, _T("%d %f %f %f %f \n"),&line->type ,&line->From.x, &line->From.y, &line->To.x, &line->To.y);

					PushLineCollider(line);
				}

			}


		}
		fclose(fpRead);
	}


}

void ServerScene::Update()
{
	for (auto& player : m_vScenePlayerList) 
	{
		player->Update();
		bool collision = false;
		static bool once = false;
		for (auto& line : m_LineColliderList)
		{

			if (line->type == COLLISION_TYPE::CT_LOPE)
			{
				if (Collision::isLineIntersectingOBB(line, player->GetCollisionData(), 30))
				{
					if (player->GetLopeUp() == 1 && !once)
					{
						once = true;
						auto t = player->GetPos();
						t.x = line->From.x;
						t.y += 30;
						player->SetPos(t);


					}
					else if (player->GetLopeUp() == 0 && !once) 
					{
						once = true;
						auto t = player->GetPos();
						t.x = line->From.x;
						t.y -= 30;
						player->SetPos(t);

					}
					
					collision = true;
				}


			}
			if (line->type == COLLISION_TYPE::CT_FLOOR)
			{
				if (Collision::PointToLine(player->GetCollisionData().GetCollisionPoint(), line) && !player->GetIsJumping())
				{
					auto ret = Collision::ClosestPoint(player->GetCollisionPoint(), line);
					auto p = player->GetPos();
					p.y = ret.y + player->GetCollisionData().GetHeight()+1; 
					player->SetPos(p);
					
					collision = true;
				}
			}

		
		}
		if (player->GetOnLope())
			player->OnLopeProc();
		else
			once=false;


		if (player->GetIsFalling())
			OutputDebugString(L"Falling\n");
		else
		{
			OutputDebugString(L"stand\n");

		}





		if (player->GetIsMove())
		{
			std::shared_ptr<Packet> pack = std::make_shared<Packet>();
			MoveStartPacket(pack, player->GetDirection(), player->GetSessionID(), player->GetPos().x,
				player->GetPos().y,
				player->GetAction(),
				player->GetIsFalling(),
				player->GetIsJumping());
			IOCPServer::GetInstance().AddPacket(pack, player->GetCurrentScene());
		}
		else
		{
			std::shared_ptr<Packet> Pack = std::make_shared<Packet>();
			MoveStopPacket(Pack, player->GetDirection(), player->GetSessionID(), player->GetPos().x,
				player->GetPos().y,
				player->GetAction(),
				player->GetIsFalling(),
				player->GetIsJumping());
			IOCPServer::GetInstance().AddPacket(Pack, player->GetCurrentScene());
		}

	}
	for (auto& mon : m_vSceneMonsterList)
	{
		mon->Update();

		std::shared_ptr<Packet> pack = std::make_shared<Packet>();
		MonsterStateUpdatePacket(pack, *mon);
		IOCPServer::GetInstance().AddPacket(pack, mon->GetCurrentScene());
	}
		

}


std::shared_ptr<ServerScene> ServerSceneMgr::InsertScene(int SceneNum)
{
	auto it = m_mSceneList.find(SceneNum);

	if (it != m_mSceneList.end())
		return it->second;
	//씬이 로딩되어 있지 않은경우 파일에서 몬스터 정보와 충돌정보를 가져와야한다...
	std::shared_ptr<ServerScene> newScene = std::make_shared<ServerScene>();


	newScene->LoadSceneData(SceneNum);


	m_mSceneList.insert({ SceneNum,newScene });


	return newScene;
}

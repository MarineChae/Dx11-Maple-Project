#include"Netstd.h"
#include"PlayerData.h"
#include "ServerScene.h"

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
					//TODO
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
					_stscanf_s(buffer, _T("%f %f %f %f \n"), &line->From.x, &line->From.y, &line->To.x, &line->To.y);

					PushLineCollider(line);
				}

			}


		}
		fclose(fpRead);
	}


}

void ServerScene::Update()
{

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

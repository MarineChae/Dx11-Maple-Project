#include"pch.h"
#include "SaveLoader.h"
#include"Scene.h"
bool SaveLoader::SaveData(std::shared_ptr<Scene> pSceneData, std::string SavePath)
{

	FILE* fpWrite = nullptr;
	if (fopen_s(&fpWrite, SavePath.c_str(), "w") == 0)
	{

		int a = 0;
	}




	return false;
}


bool SaveLoader::LoadData(std::shared_ptr<Scene> pSceneData)
{







	return false;
}

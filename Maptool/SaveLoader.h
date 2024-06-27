#pragma once
#include"Singleton.h"
class Scene;
class SaveLoader
{
private:

	std::shared_ptr<Scene> m_pSceneData;

public:
	bool SaveData(std::shared_ptr<Scene> pSceneData,std::string SavePath);
	bool LoadData(std::shared_ptr<Scene> pSceneData);



};


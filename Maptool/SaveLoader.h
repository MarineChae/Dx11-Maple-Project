#pragma once
#include"Singleton.h"

class Scene;
class MonsterObject;
class SaveLoader
{
private:

	std::shared_ptr<Scene> m_pSceneData;

public:
	bool SaveData(std::shared_ptr<Scene> pSceneData,std::string SavePath);
	bool LoadData(std::shared_ptr<Scene> pSceneData, std::string LoadPath);
	bool SaveMonsterData(std::shared_ptr<MonsterObject> monster);
	bool LoadMonsterData(std::shared_ptr<MonsterObject> monster, std::string LoadPath);


};


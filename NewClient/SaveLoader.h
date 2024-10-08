#pragma once
#include"Singleton.h"
class Scene;
class MonsterObject;
class SpriteObject;
class SaveLoader
{
private:

	std::shared_ptr<Scene> m_pSceneData;

public:
	bool SaveData(std::shared_ptr<Scene> pSceneData,std::string SavePath);
	bool LoadData(std::shared_ptr<Scene> pSceneData, std::string LoadPath);

	bool SaveMonsterData(std::shared_ptr<MonsterObject> monster);
	bool LoadMonsterData(std::shared_ptr<MonsterObject> monster, std::string LoadPath);
	bool LoadObjectData(std::shared_ptr<SpriteObject> monster, std::string LoadPath);

};

class SaveLoadMgr :public Singleton<SaveLoadMgr>
{
	private:
		SaveLoader m_SaveLoader;
	
	public:
		SaveLoader GetSaveLoader() { return m_SaveLoader; };
};

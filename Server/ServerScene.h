#pragma once
class MonsterData;
class ObjectData;
class ServerScene
{
private:
	std::vector<std::shared_ptr<PlayerData>> m_vScenePlayerList;
	std::vector<std::shared_ptr<MonsterData>> m_vSceneMonsterList;
	std::vector<std::shared_ptr<ObjectData>> m_vSceneObjectList;
	std::vector<std::shared_ptr<Line>>		  m_LineColliderList;
	float									 m_fTargetChangeTime;
public:
	std::vector<std::shared_ptr<PlayerData>> GetScenePlayerList()const { return m_vScenePlayerList; }
	std::vector<std::shared_ptr<MonsterData>> GetSceneMonsterList()const { return m_vSceneMonsterList; }
	std::vector<std::shared_ptr<ObjectData>> GetSceneObjectList()const { return m_vSceneObjectList; }
	void AddScenePlayer(std::shared_ptr<PlayerData> data);
	void DeleteScenePlayer(std::shared_ptr<PlayerData> data);


	bool LoadMonsterData(std::shared_ptr<MonsterData> monster, std::string LoadPath);
	void LoadSceneData(int Scenenum);
	std::vector<std::shared_ptr<Line>>		  GetLineColliderList() { return m_LineColliderList; }
	void PushLineCollider(std::shared_ptr <Line> line) { m_LineColliderList.push_back(line); };
	void PushObject(std::shared_ptr<ObjectData> obj) { m_vSceneObjectList.push_back(obj); };

	std::shared_ptr<MonsterData> GetMonsterData(int id) { return m_vSceneMonsterList[id]; };
public:
	void Update();

};




class ServerSceneMgr : public Singleton<ServerSceneMgr>
{

private:
	std::map<int, std::shared_ptr<ServerScene>> m_mSceneList;
	

public:
	std::map<int, std::shared_ptr<ServerScene>> GetSceneList() const { return m_mSceneList; };
	std::shared_ptr<ServerScene> InsertScene(int SceneNum);



};

#pragma once

class ServerScene
{
private:
	std::vector<std::shared_ptr<PlayerData>> m_vScenePlayerList;

	std::vector<std::shared_ptr<Line>>		  m_LineColliderList;

public:
	std::vector<std::shared_ptr<PlayerData>> GetScenePlayerList()const { return m_vScenePlayerList; }
	void AddScenePlayer(std::shared_ptr<PlayerData> data);
	void DeleteScenePlayer(std::shared_ptr<PlayerData> data);



	void LoadSceneData(int Scenenum);
	std::vector<std::shared_ptr<Line>>		  GetLineColliderList() { return m_LineColliderList; }
	void PushLineCollider(std::shared_ptr <Line> line) { m_LineColliderList.push_back(line); };
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

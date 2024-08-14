#pragma once
#include "Object.h"

class Scene
{
private:
	std::wstring m_sMapName;
	std::shared_ptr<Object> m_pMap;
	std::shared_ptr<Object> m_pBackGrondMap;
	std::vector<std::shared_ptr<Object>> m_PlayerList;
	std::vector<std::shared_ptr<Object>> m_MonsterList;
	std::vector<std::shared_ptr<Object>> m_ObjectList;
	std::vector<std::shared_ptr<Line>>   m_LineColliderList;
	std::shared_ptr<Collider> m_pCollider;
public:

	std::shared_ptr<Object> GetMap() { return m_pMap; }
	std::shared_ptr<Object> GetBackGrondMap() { return m_pBackGrondMap; }
	std::vector<std::shared_ptr<Object>> GetPlayerList() { return m_PlayerList; }
	std::vector<std::shared_ptr<Object>> GetMonsterList() { return m_MonsterList; }
	std::vector<std::shared_ptr<Object>> GetObjectList() { return m_ObjectList; }


	void PushMonster(std::shared_ptr<Object> monster ) { return m_MonsterList.push_back(monster); }
	void PushObject(std::shared_ptr<Object> object) { return m_ObjectList.push_back(object); }

	std::vector<std::shared_ptr<Line>>   GetLineColliderList() { return m_LineColliderList; }
	std::shared_ptr<Collider>GetCollider() { return m_pCollider; }
	std::wstring GetMapName() const { return  m_sMapName; }
	void PushLineCollider(std::shared_ptr <Line> line) { m_LineColliderList.push_back(line); };

	void ResetMap(std::wstring MapName);
public:
	bool Init(std::wstring MapName);
	bool Frame();
	bool Render();
	bool Release();

public:
	Scene();
	~Scene();

};


class SceneMgr :public Singleton<SceneMgr>
{
	friend class  Singleton<SceneMgr>;

private:
	std::map<std::wstring, Scene> m_mSceneList;

public:





};


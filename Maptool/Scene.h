#pragma once
#include "Object.h"

class Scene
{
private:
	std::wstring m_sMapName;
	std::shared_ptr<Object> m_pMap;
	std::shared_ptr<Object> m_pBackGrondMap;
	std::vector<Object> m_PlayerList;
	std::vector<Object> m_MonsterList;
	std::vector<Line>   m_LineColliderList;
	std::shared_ptr<Collider> m_pCollider;
public:

	std::shared_ptr<Object> GetMap() { return m_pMap; }
	std::shared_ptr<Object> GetBackGrondMap() { return m_pBackGrondMap; }
	std::vector<Object> GetPlayerList() { return m_PlayerList; }
	std::vector<Object> GetMonsterList() { return m_MonsterList; }
	std::vector<Line>   GetLineColliderList() { return m_LineColliderList; }
	std::shared_ptr<Collider>GetCollider() { return m_pCollider; }
	std::wstring GetMapName() const { return  m_sMapName; }
	void PushLineCollider(Line& line) { m_LineColliderList.push_back(line); };

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


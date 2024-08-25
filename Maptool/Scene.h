#pragma once
#include "PotalObject.h"
#include"MonsterObject.h"
class Scene
{
private:
	int										  m_iSceneNum;
	std::wstring							  m_sMapName;
	std::shared_ptr<Object>					  m_pMap;
	std::shared_ptr<Object>					  m_pBackGrondMap;
	std::shared_ptr<Collider>				  m_pCollider;
	std::vector<std::shared_ptr<Object>>	  m_PlayerList;
	std::vector<std::shared_ptr<MonsterObject>>	  m_MonsterList;
	std::vector<std::shared_ptr<Object>>	  m_InteractObjectList;
	std::vector<std::shared_ptr<Object>>	  m_ObjectList;
	std::vector<std::shared_ptr<Object>>	  m_PotalList;
	std::vector<std::shared_ptr<Line>>		  m_LineColliderList;
	
public:
	int									GetSceneNum() const { return m_iSceneNum; };
	std::wstring						GetMapName() const { return  m_sMapName; }
	std::shared_ptr<Object>				GetMap() { return m_pMap; }
	std::shared_ptr<Object>				GetBackGrondMap()const { return m_pBackGrondMap; }
	std::vector<std::shared_ptr<Object>>GetPlayerList() const { return m_PlayerList; }
	std::vector<std::shared_ptr<MonsterObject>>GetMonsterList()const { return m_MonsterList; }
	std::vector<std::shared_ptr<Object>>GetObjectList() const { return m_ObjectList; }
	std::vector<std::shared_ptr<Object>>GetInteractObjectList() const { return m_InteractObjectList; }
	std::vector<std::shared_ptr<Object>>GetPotalList() const { return m_PotalList; }
	std::shared_ptr<Collider>			GetCollider() const { return m_pCollider; }
	std::vector<std::shared_ptr<Line>>	GetLineColliderList() const { return m_LineColliderList; }

	void SetSceneNum(int num) { m_iSceneNum = num; };
	void PushMonster(std::shared_ptr<MonsterObject> monster ) { return m_MonsterList.push_back(monster); }
	void PushObject(std::shared_ptr<Object> object) { return m_ObjectList.push_back(object); }
	void PushmInteractObjectList(std::shared_ptr<Object> object) { return m_InteractObjectList.push_back(object); }
	void PushPotalObject(std::shared_ptr<Object> object) { return m_PotalList.push_back(object); }
	void PushLineCollider(std::shared_ptr <Line> line) { m_LineColliderList.push_back(line); };
	void ResetMap(std::wstring MapName);
	void ClearLineList() { m_LineColliderList.clear(); }
	void ClearPotalList() { m_PotalList.clear(); };
	void ClearObjectList() { m_ObjectList.clear(); };
	void ClearInteractObjectListt() { m_InteractObjectList.clear(); };
	void ClearMonsterList() { m_MonsterList.clear(); }
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


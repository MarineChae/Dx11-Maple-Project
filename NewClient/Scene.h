#pragma once
#include "MonsterObject.h"

class PlayerObject;
class Sound;
class Scene
{
private:
	int										  m_iSceneNum;
	std::wstring							  m_sMapName;
	std::shared_ptr<Object>					  m_pMap;
	std::shared_ptr<Object>					  m_pBackGrondMap;
	std::shared_ptr<Collider>				  m_pCollider;
	std::vector<std::shared_ptr<PlayerObject>>	  m_PlayerList;
	std::vector<std::shared_ptr<MonsterObject>>	  m_MonsterList;
	std::vector<std::shared_ptr<SpriteObject>>	  m_InteractObjectList;
	std::vector<std::shared_ptr<Object>>	  m_ObjectList;
	std::vector<std::shared_ptr<Object>>	  m_PotalList;
	std::vector<std::shared_ptr<Line>>		  m_LineColliderList;
	std::shared_ptr<Sound>					  m_pBGM;
public:
	int										  GetSceneNum() const { return m_iSceneNum; };
	std::wstring							  GetMapName() const { return  m_sMapName; }
	std::shared_ptr<Object>					  GetMap() { return m_pMap; }
	std::shared_ptr<Object>					  GetBackGrondMap() { return m_pBackGrondMap; }
	std::vector<std::shared_ptr<PlayerObject>>	  GetPlayerList() { return m_PlayerList; }
	std::vector<std::shared_ptr<MonsterObject>>	  GetMonsterList() { return m_MonsterList; }
	std::vector<std::shared_ptr<Object>>	  GetObjectList() { return m_ObjectList; }
	std::vector<std::shared_ptr<SpriteObject>>	  GetInteractObjectList() { return m_InteractObjectList; }
	std::vector<std::shared_ptr<Object>>	  GetPotalList() { return m_PotalList; }
	std::shared_ptr<Collider>				  GetCollider() { return m_pCollider; }
	std::vector<std::shared_ptr<Line>>		  GetLineColliderList() { return m_LineColliderList; }
	std::shared_ptr<Sound>					  GetBGM() { return m_pBGM; }

	void SetSceneNum(int num) { m_iSceneNum = num; };
	void SetBGM(std::shared_ptr<Sound> bgm) { m_pBGM = bgm; };

	void PushMonster(std::shared_ptr<MonsterObject> monster) { return m_MonsterList.push_back(monster); }
	void PushPlayer(std::shared_ptr<PlayerObject> object) { return m_PlayerList.push_back(object); }
	void PushObject(std::shared_ptr<Object> object) { return m_ObjectList.push_back(object); }
	void PushInteractObjectList(std::shared_ptr<SpriteObject> object) { return m_InteractObjectList.push_back(object); }
	void PushPotalObject(std::shared_ptr<Object> object) { return m_PotalList.push_back(object); }
	void PushLineCollider(std::shared_ptr <Line> line) { m_LineColliderList.push_back(line); };

	void DeletePlayer(std::shared_ptr<PlayerObject> object);

	void ResetMap(std::wstring MapName);
	void ClearPlayerList() { m_PlayerList.clear(); }
	void ClearLineList() { m_LineColliderList.clear(); }
	void ClearPotalList() { m_PotalList.clear(); };
	void ClearObjectList() { m_ObjectList.clear(); };
	void ClearMonsterList() { m_MonsterList.clear(); }
	void ClearInteractObjectList() { m_InteractObjectList.clear(); }

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
	std::map<int, std::shared_ptr<Scene>> m_mSceneList;
	std::shared_ptr<Scene> m_CurrentScene;
public:

	std::shared_ptr<Scene> GetScene(int key);
	std::shared_ptr<Scene> GetCurrentScene() { return m_CurrentScene; }
	void SetCurrentScene(std::shared_ptr<Scene> scene) { m_CurrentScene = scene; };


};


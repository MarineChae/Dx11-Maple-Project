#pragma once
#include"Core.h"
#include"PlayerObject.h"
#include"MonsterObject.h"
enum CLICK_ACTION
{
	DRAW_LINE_COLLISION,
	OBJECT_PLACE,
	MONSTER_PLACE,
	EDIT_MONSTER,
	SCENE_DATA,

};

class Scene;
class SaveLoader;
class ClientMain : public Core
{

public:
	std::shared_ptr <Scene> m_testscene;
	std::shared_ptr<Object> CurrentMap;
	std::shared_ptr<PlayerObject> test2;

	std::vector<std::shared_ptr<MonsterObject>> m_EditMonsterList;
	std::vector<std::shared_ptr<SpriteObject>> m_EditObjectList;
	std::shared_ptr<MonsterObject> m_CreateMonster;
	std::shared_ptr<MonsterObject> m_EditMonster;
	std::shared_ptr<SpriteObject> m_EditObject;
	std::shared_ptr<Object> m_pPlacedObject;
	std::shared_ptr<MonsterObject> m_pPlacedMonster;

	std::vector<PNCT_VERTEX> v;
	std::vector<Line> linelist;
	CLICK_ACTION m_ClickAction;
	std::shared_ptr<SaveLoader> m_pSaveLoader;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:

	void Menu();
	void SelectMenu();

public:
	ClientMain();
	~ClientMain();

};


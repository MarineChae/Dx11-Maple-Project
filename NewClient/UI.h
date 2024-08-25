#pragma once
#include"Singleton.h"
class Object;

enum UI_DESC
{
	HP,
	MP,
	STATE_BAR,
	BOSS_HP,
	BOSS_STATE,
	BOSS_PORTRAIT,
	BOSS_HPBAR1,
	BOSS_HPBAR2,
	BOSS_HPBAR3,


};


class UI
{
private:

	std::vector<std::shared_ptr<Object>> m_vUIList;



public:
	bool Init();
	bool Frame();
	bool Render();
	std::shared_ptr<Object> GetUIObject(UI_DESC ui) { return m_vUIList[ui]; }

public:
	UI();
	~UI();

};

class UIMgr : public Singleton<UIMgr>
{

private:
	std::shared_ptr<UI> m_pUI;


public:
	void Init();
	std::shared_ptr<UI> GetUI() { return m_pUI; }


};


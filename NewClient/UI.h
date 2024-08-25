#pragma once
#include"Singleton.h"
class Object;

enum UI_DESC
{
	HP,
	MP,
	STATE_BAR,
	BOSS_STATE,
	BOSS_PORTRAIT,
	BOSS_HP1,
	BOSS_HP2,
	BOSS_HP3,


};


class UI
{
private:

	std::vector<std::shared_ptr<Object>> m_vUIList;



public:
	bool Init();
	bool Frame();
	bool Render();


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


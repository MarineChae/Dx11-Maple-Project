#pragma once
#include"pch.h"

enum KEY_STATE
{
	KEY_NONE = 0,

	KEY_UP = 1,

	KEY_PUSH = 2,

	KEY_HOLD = 3,


};


class Input :public Singleton<Input>
{
	friend class Singleton<Input>;
private:
	TVector3 m_vOffSet = { 0,0,0 };
	POINT m_MousePos;
	POINT m_BeforeMousePos;
	DWORD m_dwKeyState[256] = { 0, };
	bool  m_bActiveInput = false;

public:
	DWORD GetKeyState(int value) const { return  value < 256 ? m_dwKeyState[value] : 0; };
	bool  IsActive() const { return m_bActiveInput; };
	void  SetActive(bool active) { m_bActiveInput = active; };
	POINT GetMousePos() { return m_MousePos; };
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:

	virtual ~Input();

};


#pragma once
#include"Core.h"
#include"PlayerObject.h"

class ClientMain : public Core
{

public:

	std::shared_ptr<Object> CurrentMap;
	std::shared_ptr<Object> test2;
	std::shared_ptr<Collider> testcol;
	std::vector<PNCT_VERTEX> v;
	std::vector<Line> linelist;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:

	void Menu();


public:
	ClientMain();
	~ClientMain();

};


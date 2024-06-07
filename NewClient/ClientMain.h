#pragma once
#include"Core.h"
#include"Object.h"

class ClientMain : public Core
{

public:

	std::shared_ptr<Object> test;
	std::list<Object> playerlist;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	ClientMain();
	~ClientMain();

};


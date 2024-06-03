#pragma once
#include"Core.h"
#include"Object.h"

class ClientMain : public Core
{

public:

	Object test;
	std::list<Object> playerlist;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ClientMain();
	~ClientMain();

};


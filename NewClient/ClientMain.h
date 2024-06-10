#pragma once
#include"Core.h"
#include"PlayerObject.h"

class ClientMain : public Core
{

public:

	std::shared_ptr<Object> test;


public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	ClientMain();
	~ClientMain();

};


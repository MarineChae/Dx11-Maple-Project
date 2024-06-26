#pragma once
#include"Core.h"
#include"PlayerObject.h"

class ClientMain : public Core
{

public:

	std::shared_ptr<Object> test;
	std::shared_ptr<Object> test2;
	std::shared_ptr<Collider> testcol;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	ClientMain();
	~ClientMain();

};


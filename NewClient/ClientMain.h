#pragma once
#include"Core.h"
#include"PlayerObject.h"
class Scene;
class SaveLoader;
class ClientMain : public Core
{

public:

	std::shared_ptr<Object> test;
	std::shared_ptr<Object> test2;
	std::shared_ptr<Collider> testcol;
	std::shared_ptr<Scene> testScene;
	std::shared_ptr<SaveLoader>saveload;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	ClientMain();
	~ClientMain();

};


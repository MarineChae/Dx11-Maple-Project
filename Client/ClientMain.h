#pragma once
#include"Core.h"
#include"DxRenderer.h"

class ClientMain : public Core
{

public:

	DxRenderer test;


public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ClientMain();
	~ClientMain();

};


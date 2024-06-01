#pragma once
#include"Core.h"

class ClientMain : public Core
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ClientMain();
	~ClientMain();

};


#pragma once
#include"Device.h"
#include"Camera.h"
#include"ClientNet.h"
class ClientNet;

class Core :public Device
{
private:
	Camera	   m_MainCamera;
	ClientNet  m_NetWork;

public:
	Camera& GetCamera() { return m_MainCamera; }


public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	bool Run();


private:
	virtual bool EngineInit();
	virtual bool EngineFrame();
	virtual bool EngineRender();
	virtual bool EngineRelease();
public:
	Core();
	virtual ~Core();


};


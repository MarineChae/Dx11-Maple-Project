#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
#include "Packet.h"
#include "MakePacket.h"
#include "ClientNet.h"
bool ClientMain::Init()
{
	//test = std::make_shared<PlayerObject>();
	//
	//test->SetScale(TVector3(100.0f, 100.0f, 1.0f));
	//test->Create(L"", L"../Shader/Defalutshader.hlsl");
	//
	//
	//ObejctMgr::GetInstance().PushObject(test);


	return true;
}

bool ClientMain::Frame()
{
	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		obj->Frame();
	}
	//test->Frame();

	return true;
}

bool ClientMain::Render()
{
	for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
	{
		obj->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
		obj->Render();
	}

  	return true;
}

bool ClientMain::Release()
{
	//networkClean();
	return true;
}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);
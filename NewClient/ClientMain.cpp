#include "pch.h"
#include "ClientMain.h"

bool ClientMain::Init()
{
	test.SetScale(TVector3(180.0f, 100.0f, 1.0f));
	test.Create(L"", L"Defalutshader.hlsl");
	

	return false;
}

bool ClientMain::Frame()
{
	test.Frame();
	

	return false;
}

bool ClientMain::Render()
{
	GetCamera().SetCameraPos(test.GetTransform());


	test.SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
	test.Render();
	return false;
}

bool ClientMain::Release()
{
	return false;
}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);
#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
bool ClientMain::Init()
{
	test.SetScale(TVector3(100.0f, 100.0f, 1.0f));
	test.Create(L"", L"Defalutshader.hlsl");
	

	return false;
}

bool ClientMain::Frame()
{
	test.Frame();
	
	if (Input::GetInstance().GetKeyState('W') >= KEY_PUSH )
	{
		TVector3 pos = test.GetTransform();
		pos.y += 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test.SetTransform(pos);
	}
	if (Input::GetInstance().GetKeyState('S') >= KEY_PUSH)
	{
		TVector3 pos = test.GetTransform();
		pos.y -= 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test.SetTransform(pos);
	}
	if (Input::GetInstance().GetKeyState('A') >= KEY_PUSH)
	{
		TVector3 pos = test.GetTransform();
		pos.x -= 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test.SetTransform(pos);
	}
	if (Input::GetInstance().GetKeyState('D') >= KEY_PUSH)
	{
		TVector3 pos = test.GetTransform();
		pos.x += 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test.SetTransform(pos);
	}



	return false;
}

bool ClientMain::Render()
{
	//GetCamera().SetCameraPos(test.GetTransform());


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
#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
#include "Packet.h"
#include "MakePacket.h"
#include "ClientNet.h"
bool ClientMain::Init()
{
	test = std::make_shared<Object>();

	test->SetScale(TVector3(100.0f, 100.0f, 1.0f));
	test->Create(L"", L"../Shader/Defalutshader.hlsl");
	
	
	return true;
}

bool ClientMain::Frame()
{
	
	test->Frame();
	Packet SendPacket;
	static bool chk = false;
	if (Input::GetInstance().GetKeyState('W') >= KEY_PUSH && !chk)
	{
		TVector3 pos = test->GetTransform();
		pos.y += 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test->SetTransform(pos);
		chk = true;
		MoveStartPacket(&SendPacket, (BYTE)1, (short)pos.x, (short)pos.y);
		NetSendPacket(&SendPacket);

	}
	if (Input::GetInstance().GetKeyState('W') == KEY_UP)
	{
		TVector3 pos = test->GetTransform();
		pos.y += 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test->SetTransform(pos);
		MoveStartPacket(&SendPacket, (BYTE)1, (short)pos.x, (short)pos.y);
		NetSendPacket(&SendPacket);
		chk = false;

	}
	if (Input::GetInstance().GetKeyState('S') >= KEY_PUSH)
	{
		TVector3 pos = test->GetTransform();
		pos.y -= 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test->SetTransform(pos);
	}
	if (Input::GetInstance().GetKeyState('A') >= KEY_PUSH)
	{
		TVector3 pos = test->GetTransform();
		pos.x -= 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test->SetTransform(pos);
	}
	if (Input::GetInstance().GetKeyState('D') >= KEY_PUSH)
	{
		TVector3 pos = test->GetTransform();
		pos.x += 1000.0f * Timer::GetInstance().GetSecPerFrame();
		test->SetTransform(pos);
	}

	static double packettime;
	packettime += Timer::GetInstance().GetSecPerFrame();

	if (packettime >= 5)
	{
		TVector3 pos = test->GetTransform();
		
		packettime -= 5.0;
	}

	return true;
}

bool ClientMain::Render()
{
	//GetCamera().SetCameraPos(test->GetTransform());
	
	test->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
	test->Render();
  	return true;
}

bool ClientMain::Release()
{
	return true;
}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);
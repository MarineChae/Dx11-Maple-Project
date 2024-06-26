#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
#include "Packet.h"
#include "MakePacket.h"
#include "ClientNet.h"
#include"Collider.h"
#include"Collision.h"

bool ClientMain::Init()
{
	
	//CurrentMap->Create(L" ", L"../Shader/Defalutshader.hlsl");
	testcol = std::make_shared<Collider>();
	testcol->Create(L" ", L"../Shader/LineDebug.hlsl");
	 
	
	v.resize(3);

	v[0].Pos = { -1003.0f,10.0f,0.0f };
	v[1].Pos = { -500.0f,10.0f,0.0f };
	
	v[2].Pos = { 100.0f,-100.0f,0.0f };
	Line l;
	l.From = v[0].Pos;
	l.To = v[1].Pos;
	linelist.push_back(l);

	Line l2;
	l2.From = v[1].Pos;
	l2.To = v[2].Pos;
	linelist.push_back(l2);

	testcol->SetVertexList(v);
	Device::GetContext()->UpdateSubresource(testcol->GetVertexBuffer().Get(), 0, 0, &v.at(0), 0, 0);
	testcol->SetScale({ 1,1,0 });
	return true;
}

bool ClientMain::Frame()
{
	if(CurrentMap!=nullptr)
	CurrentMap->Frame();
	Menu();

	POINT pos = Input::GetInstance().GetMousePos();
	std::wstring ws;
	ws+= std::to_wstring(pos.x);
	ws += L" , ";
	ws += std::to_wstring(pos.y);
	ws += L" , \n";
	OutputDebugString(ws.c_str());


	float x = (2 * (float)pos.x/ 1388) -1;
	float y = 1- ( 2 * (float)pos.y /766);



	v[v.size()-1].Pos = {x * 1388 ,y * 766,0.0f};

	if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_PUSH)
	{
		Line l;
		l.From  = v[v.size() - 1].Pos;

		v.push_back({});
		v[v.size() - 1].Pos = { x * 1388 ,y * 766,0.0f };

		l.To = v[v.size() - 1].Pos;
		testcol->SetVertexList(v);
		linelist.push_back(l);
	}

	Device::GetContext()->UpdateSubresource(testcol->GetVertexBuffer().Get(), 0, 0, &v.at(0), 0, 0);
	if (Input::GetInstance().GetKeyState(VK_HOME) >= KEY_PUSH)
	{

		GetCamera().AddZoom(Timer::GetInstance().GetSecPerFrame());

	}
	if (Input::GetInstance().GetKeyState(VK_END) >= KEY_PUSH)
	{

		GetCamera().AddZoom(-Timer::GetInstance().GetSecPerFrame());

	}
	if (Input::GetInstance().GetKeyState(VK_UP) >= KEY_PUSH)
	{
		TVector3 pos = GetCamera().GetCameraPos();
		pos.y += 500 * Timer::GetInstance().GetSecPerFrame();
		GetCamera().SetCameraPos(pos);

	}
	else if (Input::GetInstance().GetKeyState(VK_RIGHT) >= KEY_PUSH)
	{
		TVector3 pos = GetCamera().GetCameraPos();
		pos.x += 500 * Timer::GetInstance().GetSecPerFrame();
		GetCamera().SetCameraPos(pos);
	}
	else if (Input::GetInstance().GetKeyState(VK_LEFT) >= KEY_PUSH)
	{
		TVector3 pos = GetCamera().GetCameraPos();
		pos.x -= 500 * Timer::GetInstance().GetSecPerFrame();
		GetCamera().SetCameraPos(pos);
	}
	else if (Input::GetInstance().GetKeyState(VK_DOWN) >= KEY_PUSH)
	{
		TVector3 pos = GetCamera().GetCameraPos();
		pos.y -= 500 * Timer::GetInstance().GetSecPerFrame();
		GetCamera().SetCameraPos(pos);
	}
	
	if (Input::GetInstance().GetKeyState(VK_F1) >= KEY_PUSH)
	{

		test2 = std::make_shared<PlayerObject>();
		test2->Init();
		test2->SetPlayerSprite();
		test2->SetTransform(TVector3(0, 300, 0));
		test2->SetDestination(TVector3(0, 0, 0));
		test2->SetRenderState(true);
		ObejctMgr::GetInstance().SetPlayerObject(test2);
	}


	if (test2 != nullptr)
	{
		test2->Frame();

		for (auto line : linelist)
		{

			if (Collision::PointToLine(test2->GetCollider()->GetCollisionPoint(), line))
			{
				auto ret = Collision::ClosestPoint(test2->GetCollider()->GetCollisionPoint(), line);
				auto p = test2->GetTransform();
				p.y = ret.y + test2->GetCollider()->GetHeight();
				test2->SetTransform(p);

				OutputDebugString(L"dsahdolisahdaslid\n");
				
			}



		}
	}
		
	testcol->Frame();
	return true;
}

bool ClientMain::Render()
{
	if (CurrentMap != nullptr)
	{
		CurrentMap->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
		CurrentMap->Render();
	}
	testcol->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
	testcol->Render();

	if (test2 != nullptr)
	{
		test2->SetMatrix(nullptr, &GetCamera().GetView(), &GetCamera().GetProjection());
		test2->Render();
	}	
		
  	return true;
}

bool ClientMain::Release()
{
	return true;
}

void ClientMain::Menu()
{
	static bool   m_bImguiNew = false;
	static bool   m_bImguiLoad = false;
	static bool   m_bImguiSave = false;
	static bool   OpenMenu = false;
	static bool   OpenNew = false;
	static bool   OpenSave = false;
	static bool   OpenLoad = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("New", NULL))
			{
				OpenNew = true;

			}
			if (ImGui::MenuItem("Load", NULL))
			{
				OpenLoad = true;

			}

			if (ImGui::MenuItem("Save", NULL, OpenSave))
			{
				OpenSave = true;

			}


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (OpenNew)
	{

		
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", OpenNew))
		{
			OpenNew = false;
			m_bImguiNew = true;
		}

	}
	std::string filePath;
	std::string filePathName;
	if (ImGuiFileDialog::Instance()->IsOk())
	{
		filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
		filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
		ImGuiFileDialog::Instance()->Close();
	}

	if (m_bImguiNew && !filePathName.empty())
	{
		if(CurrentMap == nullptr)
			CurrentMap = std::make_shared<Object>();
		else
		{
			CurrentMap.reset();
			CurrentMap = std::make_shared<Object>();
		}
		
		std::wstring message_w;
		message_w.assign(filePathName.begin(), filePathName.end());

		CurrentMap->Create(message_w,L"../Shader/Defalutshader.hlsl");

		CurrentMap->SetScale({ 1388,768,0 });


		m_bImguiNew = false;
	}



}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);
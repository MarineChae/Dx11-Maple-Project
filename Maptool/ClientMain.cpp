#include "pch.h"
#include "ClientMain.h"
#include "Input.h"
#include "Timer.h"
#include "Packet.h"
#include "MakePacket.h"
#include "ClientNet.h"
#include"Collider.h"
#include"Collision.h"
#include"Scene.h"
#include"SaveLoader.h"
#include"Texture.h"

const int mapXsize = 5830;
const int mapYsize = 1764;

const int clientXsize = 1388;
const int clientYsize = 766;

bool ClientMain::Init()
{
	m_testscene = std::make_shared<Scene>();
	m_pSaveLoader = std::make_shared<SaveLoader>();
	m_testscene->Init(L" ");
	//CurrentMap->Create(L" ", L"../Shader/Defalutshader.hlsl");
	
	 v.resize(1);
	 
	 v[0].Pos = {  };

	//Line l;
	//l.From = v[0].Pos;
	//l.To = v[1].Pos;
	//linelist.push_back(l);


	//testcol->SetVertexList(v);
	//Device::GetContext()->UpdateSubresource(testcol->GetVertexBuffer().Get(), 0, 0, &v.at(0), 0, 0);
	//testcol->SetScale({ 1,1,0 });
	return true;
}

bool ClientMain::Frame()
{	
	Menu();
	SelectMenu();
	m_testscene->Frame();

	static bool draw = false;
	static bool draw2 = false;
	if(CurrentMap!=nullptr)
	CurrentMap->Frame();

	POINT pos = Input::GetInstance().GetMousePos();
	std::wstring ws;
	ws+= std::to_wstring(pos.x);
	ws += L" , ";
	ws += std::to_wstring(pos.y);
	ws += L" , \n";
	
	if (Input::GetInstance().GetKeyState(VK_F2) >= KEY_PUSH)
	{
		draw = true;
	}

	
	float x = (2 * (float)pos.x/ clientXsize) -1;
	float y = 1- ( 2 * (float)pos.y / clientYsize);
	if (m_ClickAction == CLICK_ACTION::DRAW_LINE_COLLISION)
	{


		if (draw)
		{
			v[v.size() - 1].Pos = { x * clientXsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().x
				,y * clientYsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().y,
				0.0f };
		}
		static Line l;
		if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_PUSH && draw)
		{
			if (!draw2)
			{
				l.From = v[v.size() - 1].Pos;
				v.push_back({});
				draw2 = true;
			}
			else
			{
				l.To = v[v.size() - 1].Pos;

				v[v.size() - 1].Pos = { x * clientXsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().x
									   ,y * clientYsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().y,
									   0.0f };
				v.push_back({});

				m_testscene->GetCollider()->SetVertexList(v);
				m_testscene->PushLineCollider(l);
				draw = false;
				draw2 = false;
			}

		}
		auto vb = m_testscene->GetCollider()->GetVertexBuffer().Get();
		auto& vl = m_testscene->GetCollider()->GetVertexList();
		Device::GetContext()->UpdateSubresource(vb, 0, 0,&vl.at(0), 0, 0);

	}
	if (m_ClickAction == CLICK_ACTION::MONSTER_PLACE)
	{
		if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_PUSH && draw)
		{
			if (m_EditMonster != nullptr)
			{
				std::shared_ptr<Object> mon = std::make_shared<Object>();
				mon->Create(m_EditMonster->GetTexture(), m_EditMonster->GetShader());
				mon->SetTransform({ x * clientXsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().x
				,y * clientYsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().y,
				0.0f });
				mon->SetScale({ static_cast<float>(mon->GetTexture()->GetWidth()),
								static_cast<float>(mon->GetTexture()->GetHeight()),0});
				m_testscene->PushMonster(mon);
			}

		}



	}



#pragma region Input
	if (Input::GetInstance().GetKeyState(VK_HOME) >= KEY_PUSH)
	{

		CameraMgr::GetInstance().GetCamera().AddZoom(Timer::GetInstance().GetSecPerFrame());

	}
	if (Input::GetInstance().GetKeyState(VK_END) >= KEY_PUSH)
	{

		CameraMgr::GetInstance().GetCamera().AddZoom(-Timer::GetInstance().GetSecPerFrame());

	}
	if (Input::GetInstance().GetKeyState(VK_UP) >= KEY_PUSH)
	{
		TVector3 pos = CameraMgr::GetInstance().GetCamera().GetCameraPos();
		pos.y += 500 * Timer::GetInstance().GetSecPerFrame();
		CameraMgr::GetInstance().GetCamera().SetCameraPos(pos);

	}
	else if (Input::GetInstance().GetKeyState(VK_RIGHT) >= KEY_PUSH)
	{
		TVector3 pos = CameraMgr::GetInstance().GetCamera().GetCameraPos();
		pos.x += 500 * Timer::GetInstance().GetSecPerFrame();
		CameraMgr::GetInstance().GetCamera().SetCameraPos(pos);
	}
	else if (Input::GetInstance().GetKeyState(VK_LEFT) >= KEY_PUSH)
	{
		TVector3 pos = CameraMgr::GetInstance().GetCamera().GetCameraPos();
		pos.x -= 500 * Timer::GetInstance().GetSecPerFrame();
		CameraMgr::GetInstance().GetCamera().SetCameraPos(pos);
	}
	else if (Input::GetInstance().GetKeyState(VK_DOWN) >= KEY_PUSH)
	{
		TVector3 pos = CameraMgr::GetInstance().GetCamera().GetCameraPos();
		pos.y -= 500 * Timer::GetInstance().GetSecPerFrame();
		CameraMgr::GetInstance().GetCamera().SetCameraPos(pos);
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
#pragma endregion


	if (test2 != nullptr)
	{
		test2->Frame();
		test2->m_bIsFalling = true;
		for (auto& line : m_testscene->GetLineColliderList())
		{

			if (Collision::PointToLine(test2->GetCollider()->GetCollisionPoint(), line))
			{
				auto ret = Collision::ClosestPoint(test2->GetCollider()->GetCollisionPoint(), line);
				auto p = test2->GetTransform();
				p.y = ret.y + test2->GetCollider()->GetHeight();
				test2->SetTransform(p);
				test2->m_bIsFalling = false;

				
			}
		}
		if (test2->m_bIsFalling)
		{
			auto pos = test2->GetTransform();
			pos.y -= Timer::GetInstance().GetSecPerFrame() * 500;
			test2->SetTransform(pos);
		}

	}
		

	return true;
}

bool ClientMain::Render()
{

	m_testscene->Render();

	if (test2 != nullptr)
	{
		test2->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
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

	std::string filePath;
	std::string filePathName;
	if (OpenNew)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", OpenNew))
		{
			OpenNew = false;
			m_bImguiNew = true;
		}
	
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			ImGuiFileDialog::Instance()->Close();
		}
	}


	if (m_bImguiNew && !filePathName.empty())
	{
		m_testscene.reset();
		m_testscene = std::make_shared<Scene>();

		std::wstring message_w;
		message_w.assign(filePathName.begin(), filePathName.end());

		m_testscene->ResetMap(message_w);

		m_testscene->GetMap()->SetScale({ static_cast<float>(m_testscene->GetMap()->GetTexture()->GetWidth()),
										  static_cast<float>(m_testscene->GetMap()->GetTexture()->GetHeight()),0});


		m_bImguiNew = false;
	}

	if (OpenSave)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", OpenSave))
		{
			OpenSave = false;
			m_bImguiSave = true;
		}
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			ImGuiFileDialog::Instance()->Close();
		}
	}
	if (OpenLoad)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", OpenLoad))
		{
			OpenLoad = false;
			m_bImguiLoad = true;
		}
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			ImGuiFileDialog::Instance()->Close();
		}
	}

	if (m_bImguiSave && !filePathName.empty())
	{
		m_bImguiSave = false;
		std::string name = "\\";
		name += wtm(m_testscene->GetMapName());
		name += ".txt";
		filePath += name;
		m_pSaveLoader->SaveData(m_testscene, filePath);
	}

	if (m_bImguiLoad && !filePathName.empty())
	{
		m_bImguiLoad = false;
		m_pSaveLoader->LoadData(m_testscene, filePathName);
	}
}

void ClientMain::SelectMenu()
{
	static Line* selectLine;
	static float tempx;
	static float tempy;
	ImVec2 ivMin = { static_cast<float>(1388) - static_cast<float>(1388) /3,0 };
	ImGui::SetNextWindowPos(ivMin);
	ImGui::Begin("MapTool Menu");

	if (ImGui::RadioButton("DRAW_LINE_COLLISION", (m_ClickAction == CLICK_ACTION::DRAW_LINE_COLLISION)))
	{
		m_ClickAction = CLICK_ACTION::DRAW_LINE_COLLISION;
	}
	if (ImGui::RadioButton("MONSTER_PLACE", (m_ClickAction == CLICK_ACTION::MONSTER_PLACE)))
	{
		m_ClickAction = CLICK_ACTION::MONSTER_PLACE;

	}//ImGui::SameLine();
	if (ImGui::RadioButton("OBJECT_PLACE", (m_ClickAction == CLICK_ACTION::OBJECT_PLACE)))
	{
		m_ClickAction = CLICK_ACTION::OBJECT_PLACE;

	}
	
	if (m_ClickAction == CLICK_ACTION::DRAW_LINE_COLLISION)
	{
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Collision Line List");
		if (ImGui::BeginListBox("## ", { 250,100 }))
		{
			for (auto& line : m_testscene->GetLineColliderList())
			{
				std::string s;
				s += " From : ";
				s += std::to_string(line.From.x);
				s += " , ";
				s += std::to_string(line.From.y);
				s += " \n To   : ";
				s += std::to_string(line.To.x);
				s += " , ";
				s += std::to_string(line.To.y);

				if (ImGui::Selectable(s.c_str()))
				{
					selectLine = &line;
					tempx = selectLine->From.x;
					tempy = selectLine->From.y;

				}



			}

			ImGui::EndListBox();
		}
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Select Collision Line");
		if (ImGui::BeginListBox("####"))
		{
			if (ImGui::BeginListBox("From"))
			{
			
				ImGui::InputFloat("X : ##", &tempx);
				ImGui::InputFloat("Y : ##", &tempy);

				ImGui::EndListBox();
			}


			ImGui::EndListBox();
		}
		if (ImGui::Button("Conform", ImVec2(60, 30)))
		{
			selectLine->From.x = tempx;
			selectLine->From.y = tempy;
		}
	}
	if (m_ClickAction == CLICK_ACTION::MONSTER_PLACE)
	{

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Monster List");
		if (ImGui::BeginListBox("## ", { 250,100 }))
		{
			for (int isize = 0; isize < m_EditMonsterList.size(); isize++)
			{
				bool clickmonster;
				auto tex = m_EditMonsterList[isize]->GetTexture();
				clickmonster = ImGui::ImageButton((ImTextureID)tex->GetSRV(), ImVec2(64, 64));
				if (clickmonster)
				{
					m_EditMonster = m_EditMonsterList[isize];
					break;
				}
			}

			ImGui::EndListBox();
		}



		ImGui::NewLine();
		std::string filePath;
		std::string filePathName;
		static bool monsterlist;
		if (ImGui::Button("Load", ImVec2(60, 30)))
		{
			monsterlist = true;
			
		}
		if (monsterlist)
		{
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", "../resource/");

			// display
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", monsterlist))
			{
				monsterlist = false;
			}
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				ImGuiFileDialog::Instance()->Close();
				std::shared_ptr<Object> monster = std::make_shared<Object>();
				std::wstring w;
				w.assign(filePathName.begin(), filePathName.end());
				monster->Create(w,L"../Shader/Defalutshader.hlsl");
				m_EditMonsterList.push_back(monster);

			}
		}
	}


	ImGui::End();
}

ClientMain::ClientMain()
{
}

ClientMain::~ClientMain()
{
}


GAME_PLAY(L"MapleStory", 1388, 766);
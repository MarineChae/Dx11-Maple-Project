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
std::unordered_map< MONSTER_STATE, std::string> monsterStateMap =
{ {MONSTER_STATE::MS_IDLE ,"MS_IDLE"},
	{MONSTER_STATE::MS_WALK ,"MS_WALK"},
	{MONSTER_STATE::MS_ATTACK ,"MS_ATTACK"},
	{MONSTER_STATE::MS_SKILL1 ,"MS_SKILL1"},
	{MONSTER_STATE::MS_SKILL2 ,"MS_SKILL2"},
		{ MONSTER_STATE::MS_SKILL3 ,"MS_SKILL3" }
};
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

	float mousePosX = x * clientXsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().x;
	float mousePosY = y * clientYsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().y;

	if (m_ClickAction == CLICK_ACTION::DRAW_LINE_COLLISION)
	{
		

		if (draw)
		{
			v[v.size() - 1].Pos = { mousePosX,mousePosY,0.0f };
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

				v[v.size() - 1].Pos = { mousePosX,mousePosY,0.0f };
				v.push_back({});

				m_testscene->GetCollider()->SetVertexList(v);
				std::shared_ptr<Line> line = std::make_shared<Line>();
				line->From = l.From;
				line->To = l.To;
				m_testscene->PushLineCollider(line);
				draw = false;
				draw2 = false;
			}
			auto vb = m_testscene->GetCollider()->GetVertexBuffer().Get();
			auto& vl = m_testscene->GetCollider()->GetVertexList();
			Device::GetContext()->UpdateSubresource(vb, 0, 0, &vl.at(0), 0, 0);
		}


	}
	if (m_ClickAction == CLICK_ACTION::MONSTER_PLACE)
	{
	
		if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_PUSH && draw)
		{
			draw = false;
			if (m_EditMonster != nullptr)
			{
				std::shared_ptr<MonsterObject> mon = std::make_shared<MonsterObject>();
				mon->Init();
				mon->Create(m_EditMonster->GetTexture(), m_EditMonster->GetShader());
				mon->SetTransform({ x * clientXsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().x
				,y * clientYsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().y,
				0.0f });
				mon->SetScale({ static_cast<float>(mon->GetTexture()->GetWidth()),
								static_cast<float>(mon->GetTexture()->GetHeight()),0});
				mon->SetSpriteInfo(m_EditMonster->GetSpriteInfo());
				mon->GetCollider()->SetTransform(mon->GetTransform());
				mon->GetCollider()->SetScale({ static_cast<float>(mon->GetTexture()->GetWidth()),
								static_cast<float>(mon->GetTexture()->GetHeight()),0 });
				mon->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
				m_testscene->PushMonster(mon);
			}

		}
		if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_HOLD && !draw)
		{
			for (auto& obj : m_testscene->GetMonsterList())
			{
				if (mousePosX >= obj->GetCollider()->GetTransform().x - obj->GetCollider()->GetWidth()
					&& mousePosX <= obj->GetCollider()->GetTransform().x + obj->GetCollider()->GetWidth()
					&& mousePosY >= obj->GetCollider()->GetTransform().y - obj->GetCollider()->GetHeight()
					&& mousePosY <= obj->GetCollider()->GetTransform().y + obj->GetCollider()->GetHeight())
				{
					obj->SetTransform({ mousePosX ,mousePosY,0 });
					obj->GetCollider()->SetTransform({ mousePosX ,mousePosY,0 });
					break;
				}

			}


		}


	}
	if (m_ClickAction == CLICK_ACTION::OBJECT_PLACE)
	{
		if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_PUSH && draw)
		{
			draw = false;
			if (m_EditObject != nullptr)
			{
				std::shared_ptr<SpriteObject> mon;
				if (m_EditObject->GetObjectType() == ObejctType::Defalut)
				{
					mon = std::make_shared<SpriteObject>();
				}
				else if (m_EditObject->GetObjectType() == ObejctType::Portal)
				{
					mon = std::make_shared<PotalObject>();
				}
				mon->Init();
				mon->SetSpriteInfo(m_EditObject->GetSpriteInfo());
				mon->Create(m_EditObject->GetTexture(), m_EditObject->GetShader());
				mon->SetTransform({ x * clientXsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().x
				,y * clientYsize + CameraMgr::GetInstance().GetCamera().GetCameraPos().y,
				0.0f });
				mon->SetObejctType(m_EditObject->GetObjectType());
				mon->SetScale(m_EditObject->GetSpriteInfo()->m_vScale);
				mon->GetCollider()->SetTransform(mon->GetTransform());
				mon->GetCollider()->SetScale(m_EditObject->GetSpriteInfo()->m_vScale);
				mon->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");

				m_testscene->PushObject(mon);
				if (mon->GetObjectType() == ObejctType::Portal)
					m_testscene->PushPotalObject(mon);
			}

		}
		if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEY_HOLD && !draw)
		{
			for (auto& obj : m_testscene->GetObjectList())
			{
				if (mousePosX >= obj->GetCollider()->GetTransform().x - obj->GetCollider()->GetWidth()
					&& mousePosX <= obj->GetCollider()->GetTransform().x + obj->GetCollider()->GetWidth()
					&& mousePosY >= obj->GetCollider()->GetTransform().y - obj->GetCollider()->GetHeight()
					&& mousePosY <= obj->GetCollider()->GetTransform().y + obj->GetCollider()->GetHeight())
				{
					obj->SetTransform({ mousePosX ,mousePosY,0 });
					obj->GetCollider()->SetTransform({ mousePosX ,mousePosY,0 });
					break;
				}

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
		OpenSave = false;
		OpenLoad = false;
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			OpenNew = false;
			
			ImGuiFileDialog::Instance()->Close();
		}
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			m_bImguiNew = true;
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
		OpenNew = false;
		OpenLoad = false;
		ImGuiFileDialog::Instance()->OpenDialog("Save", "Choose File", ".txt", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("Save", OpenSave))
		{
			OpenSave = false;
			ImGuiFileDialog::Instance()->Close();
		}
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			m_bImguiSave = true;
		
		}
	}
	if (OpenLoad)
	{
		OpenNew = false;
		OpenSave = false;
		ImGuiFileDialog::Instance()->OpenDialog("Load", "Choose File", ".txt", "../resource/");

		// display
		if (ImGuiFileDialog::Instance()->Display("Load", OpenLoad))
		{
			OpenLoad = false;
			ImGuiFileDialog::Instance()->Close();
		}
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	
			m_bImguiLoad = true;
		}
	}

	if (m_bImguiSave && !filePathName.empty())
	{
		m_bImguiSave = false;
		auto ret = m_testscene->GetMapName().find(L"../resource");
		if (!ret)
		{
			std::string name = "../resource/MapObejct/";
			name += std::to_string(m_testscene->GetSceneNum());
			name+= ".txt";
			m_pSaveLoader->SaveData(m_testscene, name);
		}
		else
		{
			std::string name = "\\";
			name += std::to_string(m_testscene->GetSceneNum());
			name += ".txt";
			filePath += name;
			m_pSaveLoader->SaveData(m_testscene, filePath);
		}

	}

	if (m_bImguiLoad && !filePathName.empty())
	{
		m_bImguiLoad = false;
		m_pSaveLoader->LoadData(m_testscene, filePathName);
		v = m_testscene->GetCollider()->GetVertexList();
	}
}

void ClientMain::SelectMenu()
{
	std::string filePath;
	std::string filePathName;
	static std::shared_ptr<Line> selectLine;
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
	if (ImGui::RadioButton("SceneData", (m_ClickAction == CLICK_ACTION::SCENE_DATA)))
	{
		m_ClickAction = CLICK_ACTION::SCENE_DATA;

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
				s += std::to_string(line->From.x);
				s += " , ";				
				s += std::to_string(line->From.y);
				s += " \n To   : ";		
				s += std::to_string(line->To.x);
				s += " , ";				
				s += std::to_string(line->To.y);

				if (ImGui::Selectable(s.c_str()))
				{
					selectLine = line;
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
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "PlacedMonster List");
		if (ImGui::BeginListBox("##", { 250,100 }))
		{
			for (int isize = 0; isize < m_testscene->GetMonsterList().size(); isize++)
			{
				bool clickmonster;
				auto tex = m_testscene->GetMonsterList()[isize]->GetMonsterName();
				std::string st = "No : ";
				st += std::to_string(isize);
				st += " ";
				st += tex;
				clickmonster = ImGui::Button(st.c_str(), ImVec2(100, 30));
				if (clickmonster)
				{
					m_pPlacedMonster = m_testscene->GetMonsterList()[isize];
					break;
				}
			}
			ImGui::EndListBox();
		}
		static std::shared_ptr<SpriteData> sprite;
		static MONSTER_STATE statenum;
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "MonsterSprite");
		if (ImGui::BeginListBox("### ", { 300,100 }))
		{
			
			if (m_pPlacedMonster != nullptr)
			{
				for (int i = 0; i < m_pPlacedMonster->GetSpriteList().size(); ++i)
				{
			
					std::string s = monsterStateMap[(MONSTER_STATE)i];
					if (ImGui::Button(s.c_str(), ImVec2(150, 30)))
					{
						if (m_pPlacedMonster->GetSpriteData(i) != nullptr)
						{
							sprite = m_pPlacedMonster->GetSpriteData(i);
							statenum = (MONSTER_STATE)i;
							m_pPlacedMonster->SetScale(sprite->m_vScale);
							m_pPlacedMonster->SetUVData(sprite->m_UVList, sprite->iRow, sprite->iCol);
							m_pPlacedMonster->SetSpriteInfo(sprite);
							m_pPlacedMonster->SetTexture(sprite->m_pTexture);
							m_pPlacedMonster->InitTexIndex();
							
							
							break;
						}

					}

				}


			}
			

			if (ImGui::Button("AddState", ImVec2(60, 30)))
			{
				if (m_pPlacedMonster != nullptr)
				{
					std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
					SpriteInfo->iCol = 1;
					SpriteInfo->iRow = 1;
					SpriteInfo->iMaxImageCount = 1;
					SpriteInfo->m_fDelay = 0.18f;
					SpriteInfo->m_vScale = { 46,68,1 };
					m_pPlacedMonster->PushSpriteData(SpriteInfo);
				}

			}

			/*if (m_pPlacedMonster != nullptr)
			{
				std::string s;

				ImGui::InputInt("Col : #", &m_pPlacedMonster->GetSpriteInfo()->iCol);
				ImGui::InputInt("Row : ##", &m_pPlacedMonster->GetSpriteInfo()->iRow);
				ImGui::InputInt("MaxImageCount : ##@", &m_pPlacedMonster->GetSpriteInfo()->iMaxImageCount);
				ImGui::InputDouble("Delay : ####", &m_pPlacedMonster->GetSpriteInfo()->m_fDelay);

				const char* itme[] = { "MS_IDLE",
									   "MS_WALK",	
									   "MS_ATTACK",	
									   "MS_SKILL1",	
									   "MS_SKILL2",	
									   "MS_SKILL3",	
									   };	
		
				static int ttemp = 0;
				ImGui::Combo("SpriteState ", &ttemp, itme, IM_ARRAYSIZE(itme));

				m_pPlacedMonster->InitTexIndex();
				if (ImGui::Button("AddState", ImVec2(60, 30)))
				{

					m_pPlacedMonster->GetSpriteInfo()->m_vScale = { static_cast<float>(m_pPlacedMonster->GetTexture()->GetWidth() / m_pPlacedMonster->GetSpriteInfo()->iCol),
																static_cast<float>(m_pPlacedMonster->GetTexture()->GetHeight() / m_pPlacedMonster->GetSpriteInfo()->iRow),
																	1 };
					auto t = m_pPlacedMonster->GetSpriteInfo()->m_vScale;
					m_pPlacedMonster->SetScale(t);
					m_pPlacedMonster->GetSpriteInfo()->m_UVList.clear();
					m_pPlacedMonster->SetUVData(m_pPlacedMonster->GetSpriteInfo()->m_UVList, m_pPlacedMonster->GetSpriteInfo()->iRow, m_pPlacedMonster->GetSpriteInfo()->iCol);
					m_pPlacedMonster->SetObejctType((ObejctType)ttemp);
					for (auto& obj : m_testscene->GetMonsterList())
					{
						obj->InitTexIndex();
					}
				}

			}*/


			ImGui::EndListBox();
		}
		static bool ChangeImage = false;

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "MonsterSpriteData");
		if (ImGui::BeginListBox("##@@## ", { 300,200 }))
		{
			if (sprite != nullptr)
			{
				std::string s;

				ImGui::InputInt("Col : #", &sprite->iCol);
				ImGui::InputInt("Row : ##", &sprite->iRow);
				ImGui::InputInt("MaxImageCount : ##@", &sprite->iMaxImageCount);
				ImGui::InputDouble("Delay : ####", &sprite->m_fDelay);

				const char* itme[] = { "Defalut","Portal", };
				static int ttemp = 0;
				ImGui::Combo("ObjectType ", &ttemp, itme, IM_ARRAYSIZE(itme));
			
	
				if (ImGui::Button("Conform", ImVec2(60, 30)))
				{
					/////이미지 불러오기는 다했음 세이브 로드 만들어라
					m_pPlacedMonster->GetSpriteInfo()->m_vScale = { static_cast<float>(m_pPlacedMonster->GetTexture()->GetWidth() / m_pPlacedMonster->GetSpriteInfo()->iCol),
																static_cast<float>(m_pPlacedMonster->GetTexture()->GetHeight() / m_pPlacedMonster->GetSpriteInfo()->iRow),
																	1 };
					auto t = m_pPlacedMonster->GetSpriteInfo()->m_vScale;
					m_pPlacedMonster->SetScale(t);
					m_pPlacedMonster->GetSpriteInfo()->m_UVList.clear();
					m_pPlacedMonster->SetUVData(m_pPlacedMonster->GetSpriteInfo()->m_UVList, m_pPlacedMonster->GetSpriteInfo()->iRow, m_pPlacedMonster->GetSpriteInfo()->iCol);
					m_pPlacedMonster->SetObejctType((ObejctType)ttemp);
					for (auto& obj : m_testscene->GetMonsterList())
					{
						obj->InitTexIndex();
					}
				}

				if (ImGui::Button("ChangeImage", ImVec2(60, 30)))
				{
					ChangeImage = true;
				}


			}
			ImGui::EndListBox();
		}

		if (ChangeImage)
		{
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", "../resource/");

			// display
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ChangeImage))
			{
				ChangeImage = false;
			}
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				ImGuiFileDialog::Instance()->Close();
				std::wstring w;
				w.assign(filePathName.begin(), filePathName.end());

				auto tex = TextureMgr::GetInstance().Load(w);

				std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
				SpriteInfo->iCol = 1;
				SpriteInfo->iRow = 1;
				SpriteInfo->iMaxImageCount = 1;
				SpriteInfo->m_fDelay = 0.18f;
				SpriteInfo->m_vScale = { 46,68,1 };
				SpriteInfo->m_pTexture = tex;
				m_pPlacedMonster->SetScale(SpriteInfo->m_vScale);
				m_pPlacedMonster->SetUVData(SpriteInfo->m_UVList, SpriteInfo->iRow, SpriteInfo->iCol);
				m_pPlacedMonster->ChangeSpriteData(SpriteInfo, statenum);
				m_pPlacedMonster->SetTexture(SpriteInfo->m_pTexture);
				m_pPlacedMonster->InitTexIndex();
			}
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
				std::shared_ptr<MonsterObject> monster = std::make_shared<MonsterObject>();
				std::wstring w;
				w.assign(filePathName.begin(), filePathName.end());
				monster->Init();
				monster->Create(w,L"../Shader/Defalutshader.hlsl");

				std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
				SpriteInfo->iCol = 1;
				SpriteInfo->iRow = 1;
				SpriteInfo->iMaxImageCount = 1;
				SpriteInfo->m_fDelay = 0.18f;
				SpriteInfo->m_vScale = { 46,68,1 };
				monster->SetSpriteInfo(SpriteInfo);
				monster->Create(w, L"../Shader/Defalutshader.hlsl");


				m_EditMonsterList.push_back(monster);



			}
		}
		static bool isCreate;
		if (ImGui::Button("Create", ImVec2(60, 30)))
		{
			isCreate = true;
			
		}
		if (isCreate)
		{
			static char g_MonsterName[64] = "";

			static bool bAccountWindow = true;
			ImGui::SetNextWindowSize(ImVec2(200, 100));
			ImGui::SetNextWindowPos(ImVec2(300, 300), 0, ImVec2(0, 0));
			ImGui::Begin("AccountWindow", &bAccountWindow);
			ImGui::Text("MonsterName : ");
			ImGui::InputText("##MonsterName", g_MonsterName, sizeof(g_MonsterName));

			if (ImGui::Button("Conform", ImVec2(60, 30)))
			{
				isCreate = false;
				std::shared_ptr<MonsterObject> monster = std::make_shared<MonsterObject>();
				monster->Init();

				std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
				SpriteInfo->iCol = 1;
				SpriteInfo->iRow = 1;
				SpriteInfo->iMaxImageCount = 1;
				SpriteInfo->m_fDelay = 0.18f;
				SpriteInfo->m_vScale = { 46,68,1 };
				monster->SetSpriteInfo(SpriteInfo);
				monster->Create(L"../resource/Monster/Defalut/Defalut.png", L"../Shader/Defalutshader.hlsl");

				monster->GetCollider()->SetTransform(monster->GetTransform());
				monster->GetCollider()->SetScale({ static_cast<float>(monster->GetTexture()->GetWidth()),
								static_cast<float>(monster->GetTexture()->GetHeight()),0 });
				monster->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
				monster->SetMonsterName(g_MonsterName);
				m_testscene->PushMonster(monster);
			}

			ImGui::End();
		}

	}
	if (m_ClickAction == CLICK_ACTION::OBJECT_PLACE)
	{

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Object List");
		if (ImGui::BeginListBox("## ", { 250,100 }))
		{
			for (int isize = 0; isize < m_EditObjectList.size(); isize++)
			{
				bool clickmonster;
				auto tex = m_EditObjectList[isize]->GetTexture();
				clickmonster = ImGui::ImageButton((ImTextureID)tex->GetSRV(), ImVec2(64, 64));
				if (clickmonster)
				{
					m_EditObject = m_EditObjectList[isize];
					break;
				}
			}
			ImGui::EndListBox();
		}

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "PlacedObject List");
		if (ImGui::BeginListBox("##", { 250,100 }))
		{
			for (int isize = 0; isize < m_testscene->GetObjectList().size(); isize++)
			{
				bool clickmonster;
				auto tex = m_testscene->GetObjectList()[isize]->GetTexture();
				std::string st = wtm(tex->GetName());
				st += isize;
				clickmonster = ImGui::Button(st.c_str(), ImVec2(100, 30));
				if (clickmonster)
				{
					m_pPlacedObject = m_testscene->GetObjectList()[isize];
					break;
				}
			}
			ImGui::EndListBox();
		}

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Object Data");
		if (ImGui::BeginListBox("##@@# ", { 250,100 }))
		{
		
			if (m_pPlacedObject != nullptr)
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Position");
				ImGui::NewLine();
				TVector3 tempPos = m_pPlacedObject->GetTransform();
				ImGui::InputFloat("X : ", &tempPos.x);
				ImGui::InputFloat("Y : ", &tempPos.y);

				m_pPlacedObject->SetTransform(tempPos);
			}
			ImGui::EndListBox();
		}

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "SpriteData");
		if (ImGui::BeginListBox("### ", { 250,100 }))
		{
			if (m_EditObject != nullptr)
			{
				std::string s;
	
				ImGui::InputInt("Col : #", &m_EditObject->GetSpriteInfo()->iCol);
				ImGui::InputInt("Row : ##", &m_EditObject->GetSpriteInfo()->iRow);
				ImGui::InputInt("MaxImageCount : ##@", &m_EditObject->GetSpriteInfo()->iMaxImageCount);
				ImGui::InputDouble("Delay : ####", &m_EditObject->GetSpriteInfo()->m_fDelay);
				
				const char* itme[] = { "Defalut","Portal", };
				static int ttemp = 0;
				ImGui::Combo("ObjectType ",&ttemp, itme,IM_ARRAYSIZE(itme));
				
				
				m_EditObject->InitTexIndex();
				if (ImGui::Button("Conform", ImVec2(60, 30)))
				{
					
					m_EditObject->GetSpriteInfo()->m_vScale = { static_cast<float>(m_EditObject->GetTexture()->GetWidth() / m_EditObject->GetSpriteInfo()->iCol),
																static_cast<float>(m_EditObject->GetTexture()->GetHeight() / m_EditObject->GetSpriteInfo()->iRow),
																	1 };
					auto t = m_EditObject->GetSpriteInfo()->m_vScale;
					m_EditObject->SetScale(t);
					m_EditObject->GetSpriteInfo()->m_UVList.clear();
					m_EditObject->SetUVData(m_EditObject->GetSpriteInfo()->m_UVList, m_EditObject->GetSpriteInfo()->iRow, m_EditObject->GetSpriteInfo()->iCol);
					m_EditObject->SetObejctType((ObejctType)ttemp);
					for (auto& obj : m_testscene->GetObjectList())
					{
						obj->InitTexIndex();
					}
				}
				
			}


			ImGui::EndListBox();
		}



		ImGui::NewLine();

		static bool ObejctList;
		if (ImGui::Button("Load", ImVec2(60, 30)))
		{
			ObejctList = true;

		}
		if (ObejctList)
		{
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", "../resource/");

			// display
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ObejctList))
			{
				ObejctList = false;
			}
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				ImGuiFileDialog::Instance()->Close();
				std::shared_ptr<SpriteObject> SpriteObejct = std::make_shared<SpriteObject>();

				std::wstring w;
				w.assign(filePathName.begin(), filePathName.end());
				std::shared_ptr<SpriteData> SpriteInfo = std::make_shared<SpriteData>();
				SpriteInfo->iCol = 1;
				SpriteInfo->iRow = 1;
				SpriteInfo->iMaxImageCount = 1;
				SpriteInfo->m_fDelay = 0.18f;
				SpriteInfo->m_vScale = { 46,68,1 };
				SpriteObejct->SetSpriteInfo(SpriteInfo);
				SpriteObejct->Create(w, L"../Shader/Defalutshader.hlsl");
		
				m_EditObjectList.push_back(SpriteObejct);

			}
		}

	}
	if (m_ClickAction == CLICK_ACTION::SCENE_DATA)
	{
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "SceneData");
		ImGui::NewLine();
		int scenenum = m_testscene->GetSceneNum();
		ImGui::InputInt("SceneNum", &scenenum);
		m_testscene->SetSceneNum(scenenum);

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Potal List");
		static std::shared_ptr<Object> potalobj;
		if (ImGui::BeginListBox("##", { 250,100 }))
		{
		
			for (int isize = 0; isize < m_testscene->GetPotalList().size(); isize++)
			{
				bool clickObj;
				auto tex = m_testscene->GetPotalList()[isize]->GetTexture();
				std::string st = std::to_string(isize);
				st += " : ";
				st += wtm(tex->GetName());
				clickObj = ImGui::Button(st.c_str(), ImVec2(100, 30));
				if (clickObj)
				{
					potalobj = m_testscene->GetPotalList()[isize];
					break;
				}

			}
			ImGui::EndListBox();
		}
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "PotalData");
		if (ImGui::BeginListBox("###@ ", { 250,100 }))
		{
			if (potalobj != nullptr)
			{
				int nextSceneNum = potalobj->GetNextSceneNum();
				ImGui::InputInt("SceneNum", &nextSceneNum);
				potalobj->SetNextSceneNum(nextSceneNum);

			}


			ImGui::EndListBox();
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
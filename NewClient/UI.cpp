#include"pch.h"
#include "UI.h"
#include"Object.h"
#include"Camera.h"
#include"Scene.h"
bool UI::Init()
{
	m_vUIList.resize(20);


	std::shared_ptr<Object> HP = std::make_shared<Object>();
	HP->Init();
	HP->Create(L"../resource/UI/mainBarhplayer.png", L"../Shader/Defalutshader.hlsl");
	HP->SetTransform({ 20, -700.0f, 1 });
	HP->SetScale({ 171,13,1 });
	m_vUIList[UI_DESC::HP]=HP;

	std::shared_ptr<Object> MP = std::make_shared<Object>();
	MP->Init();
	MP->Create(L"../resource/UI/mainBarmplayer.png", L"../Shader/Defalutshader.hlsl");
	MP->SetTransform({ 20, -730.0f, 1 });
	MP->SetScale({ 171,13,1 });
	m_vUIList[UI_DESC::MP] = MP;

	std::shared_ptr<Object> PlayerState = std::make_shared<Object>();
	PlayerState->Init();
	PlayerState->Create(L"../resource/UI/mainBar.png", L"../Shader/Defalutshader.hlsl");
	PlayerState->SetTransform({ 0,-698,1 });
	PlayerState->SetScale({ 204, 70, 1 });
	m_vUIList[UI_DESC::STATE_BAR] = PlayerState;



    std::shared_ptr<Object> BossState = std::make_shared<Object>();
    BossState->Init();
	BossState->Create(L"../resource/UI/backgrnd.png", L"../Shader/Defalutshader.hlsl");
	BossState->SetTransform({ -1040.0f, 731.0f, 1 });
	BossState->SetScale({ 37, 37, 1 });
	m_vUIList[UI_DESC::BOSS_STATE] = BossState;



    std::shared_ptr<Object> BossPortrait = std::make_shared<Object>();
    BossPortrait->Init();
	BossPortrait->Create(L"../resource/UI/UIBoss.png", L"../Shader/Defalutshader.hlsl");
	BossPortrait->SetTransform({ -1040.0f, 731.0f, 1 });
	BossPortrait->SetScale({ 27, 27, 1 });
	m_vUIList[UI_DESC::BOSS_PORTRAIT] = BossPortrait;


    std::shared_ptr<Object> BossState1 = std::make_shared<Object>();
	BossState1->Init();
	BossState1->Create(L"../resource/UI/backgrnd1.png", L"../Shader/Defalutshader.hlsl");
	BossState1->SetTransform({ -1003.0f, 715.0f, 1 });
	BossState1->SetScale({ 3, 20, 1 });
	m_vUIList[UI_DESC::BOSS_HPBAR1] = BossState1;


    std::shared_ptr<Object> BossState2 = std::make_shared<Object>();
	BossState2->Init();
	BossState2->Create(L"../resource/UI/backgrnd3.png", L"../Shader/Defalutshader.hlsl");
	BossState2->SetTransform({ 0.0f, 715.0f, 1 });
	BossState2->SetScale({ 1000, 20, 1 });
	m_vUIList[UI_DESC::BOSS_HPBAR2] = BossState2;

    std::shared_ptr<Object> BossState3 = std::make_shared<Object>();
	BossState3->Init();
	BossState3->Create(L"../resource/UI/backgrnd4.png", L"../Shader/Defalutshader.hlsl");
	BossState3->SetTransform({ 1003.0f, 715.0f, 1 });
	BossState3->SetScale({ 3, 20, 1 });
	m_vUIList[UI_DESC::BOSS_HPBAR3] = BossState3;


	std::shared_ptr<Object> BossHP = std::make_shared<Object>();
	BossHP->Init();
	BossHP->Create(L"../resource/UI/HPBar.png", L"../Shader/Defalutshader.hlsl");
	BossHP->SetTransform({ 1003.0f, 715.0f, 1 });
	BossHP->SetScale({ 3, 20, 1 });
	m_vUIList[UI_DESC::BOSS_HP] = BossHP;


	for (auto& ui : m_vUIList)
	{
		if(ui !=nullptr)
		ui->SetRenderState(true);
	}

	return false;
}

bool UI::Frame()
{
	if (SceneMgr::GetInstance().GetCurrentScene()->GetSceneNum() > 1)
	{
		for (int i = UI_DESC::BOSS_HP; i <= UI_DESC::BOSS_HPBAR3; ++i)
		{
			m_vUIList[i]->SetRenderState(true);
		}

	}
	else
	{
		for (int i = UI_DESC::BOSS_HP; i <= UI_DESC::BOSS_HPBAR3; ++i)
		{
			m_vUIList[i]->SetRenderState(false);
		}
	}
	for (auto& ui : m_vUIList)
	{
		if (ui != nullptr)
		if(ui->GetRenderState())
			ui->Frame();
	}


	return false;
}

bool UI::Render()
{

	for (auto& ui : m_vUIList)
	{
		if (ui != nullptr)
		if (ui->GetRenderState())
		{
			ui->SetMatrix(nullptr, nullptr, &CameraMgr::GetInstance().GetCamera().GetProjection());
			ui->Render();
		}

	}




	return false;
}

UI::UI()
{


}

UI::~UI()
{
}

void UIMgr::Init()
{
	m_pUI = std::make_shared<UI>();
	m_pUI->Init();
}

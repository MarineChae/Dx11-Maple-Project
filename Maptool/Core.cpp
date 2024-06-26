#include "pch.h"
#include "Core.h"
#include "Input.h"
#include "Timer.h"
#include "ClientNet.h"
#include"ImGuiManager.h"
bool Core::Init()
{
    return true;
}

bool Core::Frame()
{
    return true;
}

bool Core::Render()
{
    return true;
}

bool Core::Release()
{
    return true;
}

bool Core::EngineInit()
{
    
    Device::Init();
    m_MainCamera.CreateCamera(TVector3(0,0,0), TVector2(1388,766));
    m_MainCamera.Init();
    Input::GetInstance().Init();
    Timer::GetInstance().Init();

    ImGuiManager::GetInstance().Init();
    Init();

    return true;
}

bool Core::EngineFrame()
{

    Device::Frame();
    m_MainCamera.Frame();
    Input::GetInstance().Frame();
    Timer::GetInstance().Frame();
 
    ImGuiManager::GetInstance().Frame();

    SetWindowText(MyWindow::GetHWND(), std::to_wstring(Timer::GetInstance().GetFPS()).c_str());


    Frame();
    return true;
}

bool Core::EngineRender()
{
    Device::PreRender();
    GetContext()->OMSetBlendState(GetBlendState().Get(), 0, -1);
    m_MainCamera.Render();
    Input::GetInstance().Render();
    Timer::GetInstance().Render();

    ImGuiManager::GetInstance().Render();
    Render();
    
    Device::PostRender();
    return true;
}

bool Core::EngineRelease()
{
    Device::Release();
    m_MainCamera.Release();
    Release();
    return true;
}


bool Core::Run()
{
   
    if (!EngineInit())
    {
        EngineRelease();
        return false;
    }

    MSG msg = { 0, };
    while (msg.message != WM_QUIT)
    {
       
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
           
            EngineFrame();
            EngineRender();
            //게임로직 처리
        }


    }
    EngineRelease();
    return true;
}


Core::Core()
{
}

Core::~Core()
{
}

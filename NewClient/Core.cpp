#include "pch.h"
#include "Core.h"
#include "Input.h"
#include "Timer.h"
#include "ClientNet.h"

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
    CameraMgr::GetInstance().SetCamera(m_MainCamera);
    CameraMgr::GetInstance().GetCamera().Init();
    Input::GetInstance().Init();
    Timer::GetInstance().Init();
    if (!connentNetWork())
    {
        return false;
    }
   
    Init();
  
    return true;
}

bool Core::EngineFrame()
{
    Device::Frame();
    CameraMgr::GetInstance().GetCamera().Frame();
    Input::GetInstance().Frame();
    Timer::GetInstance().Frame();



    SetWindowText(MyWindow::GetHWND(), std::to_wstring(Timer::GetInstance().GetFPS()).c_str());


    Frame();
    return true;
}

bool Core::EngineRender()
{
    Device::PreRender();
    GetContext()->OMSetBlendState(GetBlendState().Get(), 0, -1);
    CameraMgr::GetInstance().GetCamera().Render();
    Input::GetInstance().Render();
    Timer::GetInstance().Render();

  
    Render();
    Device::PostRender();
    return true;
}

bool Core::EngineRelease()
{
    Device::Release();
    CameraMgr::GetInstance().GetCamera().Release();
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

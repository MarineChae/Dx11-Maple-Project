#include "pch.h"
#include "Core.h"
#include "Windows.h"
bool Core::Init()
{
    return false;
}

bool Core::Frame()
{
    return false;
}

bool Core::Render()
{
    return false;
}

bool Core::Release()
{
    return false;
}

bool Core::EngineInit()
{
    Device::Init();
    Init();
    return false;
}

bool Core::EngineFrame()
{
    Device::Frame();
    Frame();
    return false;
}

bool Core::EngineRender()
{
    Device::Render();
    Render();
    return false;
}

bool Core::EngineRelease()
{
    Device::Release();
    Release();
    return false;
}


bool Core::Run()
{
    EngineInit();

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

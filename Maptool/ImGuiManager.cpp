
#include"pch.h"
#include"Device.h"
#include "ImGuiManager.h"


bool ImGuiManager::Init()
{ // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(MyWindow::GetHWND());
    ImGui_ImplDX11_Init(Device::GetDevice(), Device::GetContext());
    return true;
}

bool ImGuiManager::Frame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
	test();
    return true;
}

bool ImGuiManager::Render()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}

bool ImGuiManager::Release()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return true;
}

bool ImGuiManager::test()
{        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	
	return false;
}

ImGuiManager::ImGuiManager()
{
}

ImGuiManager::~ImGuiManager()
{
}

#pragma once
#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <chrono>
#include <d3dcompiler.h>
#include <wincodec.h>
#include <crtdbg.h>
#include <wrl.h>
#include <memory>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <map>
#include <string>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <system_error>
#include<list>
#include<algorithm>
#include"Singleton.h"
#include "DirectXTex.h"
#include "TMath.h"
#include<codecvt>

#pragma comment (lib , "winmm.lib")
#pragma comment (lib , "d3d11.lib")
#pragma comment (lib , "dxgi.lib")
#pragma comment (lib , "D3DCompiler.lib")
#pragma comment (lib , "ws2_32.lib")
#pragma comment( lib, "dxguid.lib")
#ifdef _DEBUG
#pragma comment (lib, "DirectXTex_debug.lib")
#else
#pragma comment (lib, "DirectXTex.lib")
#endif

using namespace Microsoft::WRL;


#define WM_NETWORKMSG (WM_USER+1)

#define	dfNETWORK_UM	 WM_USER + 89


#define GAME_INIT int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow){ ClientMain window;
#define GAME_REGISTER 				     window.SetRegisterClassWindow(hInstance);
#define GAME_WINDOW(Title,Width,Height)  window.SetWindow(Title, Width, Height);
#define GAME_RUN 						 window.Run(); 
#define GAME_END					     return 0;}

#define GAME_PLAY(Title,Width,Height)  GAME_INIT GAME_REGISTER GAME_WINDOW(Title,Width,Height) GAME_RUN GAME_END

struct ConstantData
{
    TMatrix  WolrdMatrix;
    TMatrix  ViewMatrix;
    TMatrix  ProjMatrix;
    TVector4   MeshColor = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct PNCT_VERTEX
{

    TVector3 Pos;
    TVector3 Nor;
    TVector4 Col;
    TVector2 Tex;

    PNCT_VERTEX(){}
    PNCT_VERTEX(TVector3 vp, TVector3 vn,TVector4 vc,TVector2 vt)
        :Pos(vp) , Nor(vn), Col(vc), Tex(vt)
    {}
    bool operator==(const PNCT_VERTEX& other) const
    {
        return Pos == other.Pos && Nor == other.Nor && Col == other.Col && Tex == other.Tex;
    }



};

struct UVRect
{
    TVector2 m_vMin;
    TVector2 m_vMax;
};
struct Line
{
    TVector3 From;
    TVector3 To;
};

enum PLAYER_STATE
{
    PS_STAND,
    PS_WALK,
    PS_JUMP,
    PS_ATTACK,
    PS_DEFAULT,


};

enum COLLISION_TYPE
{
    CT_FLOOR,
    CT_LOPE,
    CT_POTAL,
    CT_ATTACK,
    CT_DEFAULT
};


struct MyOV 
{
    enum { MODE_RECV = 0, MODE_SEND = 1, };
    OVERLAPPED ov;
    int flag;

    MyOV(int value) : flag(value)
    {
        ZeroMemory(&ov, sizeof(OVERLAPPED));
    }
    MyOV()
    {
        ZeroMemory(&ov, sizeof(OVERLAPPED));
        flag = MyOV::MODE_RECV;
    }
};
static std::wstring mtw(std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(str);
}

static std::string wtm(std::wstring str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(str);
}


#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include<WinSock2.h>
#include <thread>
#include "DirectXTex.h"
#include<d3dcompiler.h>
#include <wincodec.h>
#include <crtdbg.h>
#include<wrl.h>
#include<memory>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include"TMath.h"
#include <windows.h>
#include<map>
#include<string>
#include<tchar.h>


#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"D3DCompiler.lib")

#ifdef _DEBUG
#pragma comment (lib, "DirectXTex_debug.lib")
#else
#pragma comment (lib, "DirectXTex.lib")
#endif

using namespace Microsoft::WRL;



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


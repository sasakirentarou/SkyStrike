#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <functional>
//マウスホイール用
#include<WinUser.h>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

//XMベクトルテスト
//#include <DirectXMath.h>
//using namespace DirectX;


#pragma warning(pop)

//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

#if 0
#define SCREEN_WIDTH	(1280)//1280
#define SCREEN_HEIGHT	(720)//720

#else
#define SCREEN_WIDTH	(1920)//1920
#define SCREEN_HEIGHT	(1080)//1080

#endif // 0

#define DEPTH_PIXEL (4096)
#define REFLECT_SIZE (512)//512

HWND GetWindow();


float GetMouseX();
float GetMouseY();

float GetMouse2DX();
float GetMouse2DY();
float GetMouseHwheel();

void SetFrame(int frame);
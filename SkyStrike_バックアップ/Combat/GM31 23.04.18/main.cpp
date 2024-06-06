#include "main.h"
#include "manager.h"
#include <thread>
#include "input.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "SkyStrike";

//グローバル宣言
POINT g_LastMousePos; //マウスポジション保存用
POINT g_CursorPos;	//マウスポジション
POINT g_CursorPos2D;

bool g_Pause = false;
int g_Frame = 60;
float g_Hweel; //回転量加算用
short g_Rot;//マウスホイール回転量


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);

		//Windowサイズ格納
		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };

		//タスクバーなどを考慮しサイズ調整
		//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		//Windowサイズ設定
		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}



	Manager::Init();

	//cursor
	GetCursorPos(&g_LastMousePos);
	ShowCursor(false);


	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);




	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;



	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / g_Frame))
			{
				dwExecLastTime = dwCurrentTime;

				GetCursorPos(&g_CursorPos);

				if (!g_Pause) {
					SetCursorPos(g_LastMousePos.x, g_LastMousePos.y);
					ShowCursor(false);
				}
				else
				{
					ShowCursor(true);
				}

				if (Input::GetKeyTrigger('O'))
				{
					g_Pause = !g_Pause;
				}

				Manager::Update();
				Manager::Draw();
			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	return (int)msg.wParam;
}

//ImGUIのウィンドウ処理をプロシージャに追加
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ImGUIのウィンドウ処理をプロシージャに追加
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEWHEEL:
		g_Rot = GET_WHEEL_DELTA_WPARAM(wParam);

		g_Hweel += -g_Rot;

		if (g_Hweel < 15.0f * 100)
		{
			g_Hweel = 15.0f * 100;
		}
		else if (g_Hweel > 100.0f * 100)
		{
			g_Hweel = 100.0f * 100;
		}

		break;

	default:
		break;
	}
	GetCursorPos(&g_CursorPos2D);
	ScreenToClient(hWnd,&g_CursorPos2D);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


float GetMouseX()
{
	return g_CursorPos.x - g_LastMousePos.x;
}

float GetMouseY()
{
	return g_CursorPos.y - g_LastMousePos.y;
}

float GetMouse2DX()
{
	return g_CursorPos2D.x;
}

float GetMouse2DY()
{
	return g_CursorPos2D.y;
}

float GetMouseHwheel()
{
	return g_Hweel;
}

void SetFrame(int frame)
{
	g_Frame = frame;
}

void SetPause(bool pause)
{
	g_Pause = pause;
}

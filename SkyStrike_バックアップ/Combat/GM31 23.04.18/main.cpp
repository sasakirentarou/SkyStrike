#include "main.h"
#include "manager.h"
#include <thread>
#include "input.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";

POINT lastMousePos; //マウスポジション保存用
POINT CursorPos;	//マウスポジション
POINT CursorPos2D;
bool Pause = false;
float hweel; //回転量加算用
short rot;//マウスホイール回転量
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
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		//Windowサイズ設定
		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}



	Manager::Init();

	//cursor
	GetCursorPos(&lastMousePos);
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

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				GetCursorPos(&CursorPos);

				if (!Pause) {
					SetCursorPos(lastMousePos.x, lastMousePos.y);
				}
				if (Input::GetKeyTrigger('O'))
				{
					Pause = true;
					ShowCursor(true);
				}
				if (Input::GetKeyTrigger('I'))
				{
					Pause = false;
					ShowCursor(false);
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
		rot = GET_WHEEL_DELTA_WPARAM(wParam);

		hweel += -rot;

		if (hweel < 15.0f * 100)
		{
			hweel = 15.0f * 100;
		}
		else if (hweel > 100.0f * 100)
		{
			hweel = 100.0f * 100;
		}

		break;

	default:
		break;
	}
	GetCursorPos(&CursorPos2D);
	ScreenToClient(hWnd,&CursorPos2D);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


float GetMouseX()
{
	return CursorPos.x - lastMousePos.x;
}

float GetMouseY()
{
	return CursorPos.y - lastMousePos.y;
}

float GetMouse2DX()
{
	return CursorPos2D.x;
}

float GetMouse2DY()
{
	return CursorPos2D.y;
}

float GetMouseHwheel()
{
	return hweel;
}

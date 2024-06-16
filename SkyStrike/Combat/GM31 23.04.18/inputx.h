//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#pragma once

#include <XInput.h>
#pragma comment(lib,"Xinput.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//ボタン一覧

// XINPUT_GAMEPAD_DPAD_UP			十字キー上
// XINPUT_GAMEPAD_DPAD_DOWN			十字キー下
// XINPUT_GAMEPAD_DPAD_LEFT			十字キー左
// XINPUT_GAMEPAD_DPAD_RIGHT		十字キー右
// XINPUT_GAMEPAD_START
// XINPUT_GAMEPAD_BACK
// XINPUT_GAMEPAD_LEFT_THUMB		Lスティック押し込み
// XINPUT_GAMEPAD_RIGHT_THUMB		Rスティック押し込み
// XINPUT_GAMEPAD_LEFT_SHOULDER		Lボタン
// XINPUT_GAMEPAD_RIGHT_SHOULDER	Rボタン
// XINPUT_GAMEPAD_A
// XINPUT_GAMEPAD_B
// XINPUT_GAMEPAD_X
// XINPUT_GAMEPAD_Y

//スティックの範囲
// X軸 左-1.0　右 1.0
// Y軸 上 1.0　下-1.0

//トリガーの範囲
// 0 ～ 255

// バイブレーションの範囲
// 0 ～ 255

#if 1
#define DEADZONE			8800	// 各軸の25%を無効ゾーンとする
#else
#define DEADZONE			1760	// 各軸の5%を無効ゾーンとする
#endif // 0

#define MAX_CONTROLLERS     4

struct CONTROLER_STATE
{
	XINPUT_STATE		lastState;
	XINPUT_STATE		state;
	XINPUT_STATE		trigger;
	XINPUT_VIBRATION	vibration;
};

class InputX
{
private:
	static CONTROLER_STATE m_Controllers[MAX_CONTROLLERS];
	static float	m_LeftStickX[MAX_CONTROLLERS];
	static float	m_LeftStickY[MAX_CONTROLLERS];
	static float	m_RightStickX[MAX_CONTROLLERS];
	static float	m_RightStickY[MAX_CONTROLLERS];

public:
	static void Init();
	static void Uninit();
	static void Update();

	//スティック
	static float GetThumbLeftX(int padNo);
	static float GetThumbLeftY(int padNo);
	static float GetThumbRightX(int padNo);
	static float GetThumbRightY(int padNo);

	//振動
	static void SetLeftVibration(int padNo, int speed);
	static void SetRightVibration(int padNo, int speed);
	static void SetVibration(int padNo, int speed);
	static void StopVibration(int padNo);

	//トリガー
	static int GetLeftTrigger(int padNo); //LT
	static int GetRightTrigger(int padNo); //RT

	//ボタン
	static BOOL IsButtonPressed(int padNo, DWORD button);
	static BOOL IsButtonTriggered(int padNo, DWORD button);
};
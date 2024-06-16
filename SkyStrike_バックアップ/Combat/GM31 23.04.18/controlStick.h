#pragma once
#include "gameObject.h"

#define PITCH_MAX_SPEED		0.015		//ピッチ(X)回転の最大速度
#define ROLL_MAX_SPEED		0.04		//ロール(Z)回転の最大速度
#define YAW_MAX_SPEED		0.005		//ヨー(Y)回転の最大速度


class ControlStick
{
private:
	D3DXQUATERNION m_Endquat = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXQUATERNION m_Quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
public:
	void Yaw(GameObject* obj, float sensitivity);
	void Pitch(GameObject* obj, float sensitivity);
	void Roll(GameObject* obj, float sensitivity);
};
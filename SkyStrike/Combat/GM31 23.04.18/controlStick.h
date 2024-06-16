#pragma once
#include "gameObject.h"

#define PITCH_MAX_SPEED		0.015		//�s�b�`(X)��]�̍ő呬�x
#define ROLL_MAX_SPEED		0.04		//���[��(Z)��]�̍ő呬�x
#define YAW_MAX_SPEED		0.005		//���[(Y)��]�̍ő呬�x


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
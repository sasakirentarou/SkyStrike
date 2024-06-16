#include "main.h"
#include "controlStick.h"


void ControlStick::Yaw(GameObject* obj, float sensitivity)
{
	float angle = 0.0f;

	angle = YAW_MAX_SPEED * sensitivity;

	m_Quaternion = obj->GetQuaternion();

	D3DXVECTOR3 axis = obj->GetTopQuaternion(); // YŽ²
	D3DXQUATERNION quat;

	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	m_Endquat = m_Endquat * quat;
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);

	obj->SetQuaternion(m_Quaternion);
}

void ControlStick::Pitch(GameObject* obj, float sensitivity)
{
	float angle = 0.0f;

	angle = PITCH_MAX_SPEED * sensitivity;

	m_Quaternion = obj->GetQuaternion();

	D3DXVECTOR3 axis = obj->GetRightQuaternion(); // XŽ²
	D3DXQUATERNION quat;

	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	m_Endquat = m_Endquat * quat;
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.1f);

	obj->SetQuaternion(m_Quaternion);
}

void ControlStick::Roll(GameObject* obj, float sensitivity)
{
	float angle = 0.0f;

	angle = ROLL_MAX_SPEED * sensitivity;

	m_Quaternion = obj->GetQuaternion();

	D3DXVECTOR3 axis = obj->GetForwardQuaternion(); // ZŽ²
	D3DXQUATERNION quat;

	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	m_Endquat = m_Endquat * quat;
	D3DXQuaternionSlerp(&m_Quaternion, &m_Quaternion, &m_Endquat, 0.001f);

	obj->SetQuaternion(m_Quaternion);
}

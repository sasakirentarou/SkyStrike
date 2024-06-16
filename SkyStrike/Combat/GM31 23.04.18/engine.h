#pragma once
#include "gameObject.h"

class Engine
{
private:
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
public:
	void Active(GameObject* obj, float speed, float sensitivity);
	void ReverseActive(GameObject* obj, float speed, float sensitivity);
};
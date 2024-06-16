#include "main.h"
#include "engine.h"
#include "engineFire.h"

void Engine::Active(GameObject* obj, float speed, float sensitivity)
{
	m_Velocity = obj->GetVelocity();
	m_Velocity += (obj->GetForwardQuaternion() * speed) * sensitivity;
	obj->SetVelocity(m_Velocity);
}

void Engine::ReverseActive(GameObject* obj, float speed, float sensitivity)
{
	m_Velocity = obj->GetVelocity();
	m_Velocity -= (obj->GetForwardQuaternion() * speed) * sensitivity;
	obj->SetVelocity(m_Velocity);
}

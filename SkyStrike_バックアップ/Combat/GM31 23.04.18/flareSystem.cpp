#include "main.h"
#include "flareSystem.h"

#include "scene.h"
#include "manager.h"
#include "flare.h"

void FlareSystem::Init()
{
	m_Amount = MAX_FLARE;
	m_Scene = Manager::GetScene();
}

void FlareSystem::Active(GameObject* obj)
{
	if (m_Time == 0)
	{
		m_Amount--;
		m_Finish = false;

		Fire(obj);
	}
	else if (m_Time == FLARE_TIME)
	{
		Fire(obj);
	}
	else if (m_Time == FLARE_TIME * 2)
	{
		Fire(obj);
	}
	else if (m_Time == FLARE_TIME * 3)
	{
		Fire(obj);
	}
	else if (m_Time > FLARE_TIME * 8)
	{
		m_Time = 0;
		m_Finish = true;
		return;
	}

	m_Time++;
}

void FlareSystem::Fire(GameObject* obj)
{
	for (int i = 0; i < 4; i++)
	{
		Flare* flare = m_Scene->AddGameObject<Flare>(1);

		//-10‚©‚ç10‚Ì”ÍˆÍ
		float rightRandom = (float)rand() / RAND_MAX * 20.0f - 10.0f;
		//5‚©‚ç15‚Ì”ÍˆÍ
		float forwardRandom = rand() % 11 + 5;

		flare->SetPosition(obj->GetPosition() + obj->LocalVector(obj->GetQuaternion(),D3DXVECTOR3(0.0f,-2.0f,-2.0f)));
		flare->SetVelocity(obj->LocalVector(obj->GetQuaternion(), D3DXVECTOR3(rightRandom,0.0f, forwardRandom)));
	}
}

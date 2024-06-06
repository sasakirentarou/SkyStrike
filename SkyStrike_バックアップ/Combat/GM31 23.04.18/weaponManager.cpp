#include "main.h"
#include "weaponManager.h"
#include "scene.h"
#include "manager.h"
#include "explosion.h"

void WeaponManager::MisiileBom()
{
	Scene* scene = Manager::GetScene();

	auto explosion = scene->AddGameObject<Explosion>(1);
	explosion->Spawn(m_Position, D3DXVECTOR2(4.5f, 4.5f), 2);

	m_ShotFlg = false;

	// Á‚·—\–ñ‚ğ‚·‚é
	SetDestroy();
}
#include "main.h"
#include "enemyManager.h"


int EnemyManager::m_EnemyCount = 0;

void EnemyManager::HealthDamage(int damage)
{
	m_Health -= damage;

	if (m_Health <= 0)
		m_CrashFlg = true;
}

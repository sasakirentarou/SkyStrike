#pragma once
#include "gameObject.h"


class EnemyManager : public GameObject
{
protected:
	static int m_EnemyCount;	// enemyJEg
	int m_EnemyID{};			//ΒΚID
	int m_Health{};				//ΜΝ
	int m_ShotCount{};			// ­ΛN[^C

	bool m_CrashFlg{};			// jσ»ΚtO
	bool m_Shot = true;			// ­ΛtO
public:
	virtual void Attack(){};	//U
	virtual void Seach(){};		//{υ

	int GetMaxEnemy() { return m_EnemyCount; }
	int GetEnemyID() { return m_EnemyID; }
	int GetHealth() { return m_Health; }
	bool GetCrash() { return m_CrashFlg; }

	
	void HealthDamage(int damage);
};


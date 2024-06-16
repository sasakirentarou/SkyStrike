#pragma once
#include "gameObject.h"


class EnemyManager : public GameObject
{
protected:
	static int m_EnemyCount;	// enemy数カウント
	int m_EnemyID{};			//個別ID
	int m_Health{};				//体力
	int m_ShotCount{};			// 発射クールタイム

	bool m_CrashFlg{};			// 破壊判別フラグ
	bool m_Shot = true;			// 発射フラグ
public:
	virtual void Attack(){};	//攻撃
	virtual void Seach(){};		//捜索

	int GetMaxEnemy() { return m_EnemyCount; }
	int GetEnemyID() { return m_EnemyID; }
	int GetHealth() { return m_Health; }
	bool GetCrash() { return m_CrashFlg; }

	
	void HealthDamage(int damage);
};


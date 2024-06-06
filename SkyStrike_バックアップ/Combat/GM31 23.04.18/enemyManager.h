#pragma once
#include "gameObject.h"

enum ENEMY_STATE
{
	ENEMY_ATTACK,		// U
	ENEMY_ESCAPE,		// ήπ
	ENEMY_SEACH,		// Tυ
	ENAMY_GROUNDRISK,	// Δρπ
	ENEMY_OUT_OF_RANGE	// ΝΝO
};

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
	virtual void Escape(){};	//Pή
	virtual void Seach(){};		//{υ
	virtual void GroundRisk(){};//nΚρπ
	virtual void OutOfRange(){};//ςsΝΝ


	int GetMaxEnemy() { return m_EnemyCount; }
	int GetEnemyID() { return m_EnemyID; }
	int GetHealth() { return m_Health; }
	bool GetCrash() { return m_CrashFlg; }

	
	void HealthDamage(int damage);
};


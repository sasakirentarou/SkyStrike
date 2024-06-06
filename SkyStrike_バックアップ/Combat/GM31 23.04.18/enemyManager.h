#pragma once
#include "gameObject.h"

enum ENEMY_STATE
{
	ENEMY_ATTACK,		// 攻撃
	ENEMY_ESCAPE,		// 退避
	ENEMY_SEACH,		// 探索
	ENAMY_GROUNDRISK,	// 墜落回避
	ENEMY_OUT_OF_RANGE	// 範囲外
};

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
	virtual void Escape(){};	//撤退
	virtual void Seach(){};		//捜索
	virtual void GroundRisk(){};//地面回避
	virtual void OutOfRange(){};//飛行範囲


	int GetMaxEnemy() { return m_EnemyCount; }
	int GetEnemyID() { return m_EnemyID; }
	int GetHealth() { return m_Health; }
	bool GetCrash() { return m_CrashFlg; }

	
	void HealthDamage(int damage);
};


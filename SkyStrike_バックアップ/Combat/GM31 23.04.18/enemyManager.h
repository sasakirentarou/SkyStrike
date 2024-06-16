#pragma once
#include "gameObject.h"


class EnemyManager : public GameObject
{
protected:
	static int m_EnemyCount;	// enemy���J�E���g
	int m_EnemyID{};			//��ID
	int m_Health{};				//�̗�
	int m_ShotCount{};			// ���˃N�[���^�C��

	bool m_CrashFlg{};			// �j�󔻕ʃt���O
	bool m_Shot = true;			// ���˃t���O
public:
	virtual void Attack(){};	//�U��
	virtual void Seach(){};		//�{��

	int GetMaxEnemy() { return m_EnemyCount; }
	int GetEnemyID() { return m_EnemyID; }
	int GetHealth() { return m_Health; }
	bool GetCrash() { return m_CrashFlg; }

	
	void HealthDamage(int damage);
};


#pragma once
#include "gameObject.h"

enum ENEMY_STATE
{
	ENEMY_ATTACK,		// �U��
	ENEMY_ESCAPE,		// �ޔ�
	ENEMY_SEACH,		// �T��
	ENAMY_GROUNDRISK,	// �ė����
	ENEMY_OUT_OF_RANGE	// �͈͊O
};

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
	virtual void Escape(){};	//�P��
	virtual void Seach(){};		//�{��
	virtual void GroundRisk(){};//�n�ʉ��
	virtual void OutOfRange(){};//��s�͈�


	int GetMaxEnemy() { return m_EnemyCount; }
	int GetEnemyID() { return m_EnemyID; }
	int GetHealth() { return m_Health; }
	bool GetCrash() { return m_CrashFlg; }

	
	void HealthDamage(int damage);
};


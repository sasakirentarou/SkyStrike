#pragma once

#include "gameObject.h"
#include "model.h"
#include <string>

#define ROTATION_AMOUNT		0.05 //�@�̂̉�]�� �����������������nan�ɂȂ�0.015
#define MISSILE_COOLDOWN	6	 //�~�T�C���̃N�[���_�E��
#define MAX_HEALTH			100	 //�ő�̗�

enum ENEMY_STATE
{
	ENEMY_ATTACK,	// �U��
	ENEMY_ESCAPE,	// �ޔ�
	ENEMY_SEACH,	// �T��
	ENAMY_GROUNDRISK// �ė����
};

class Enemy : public GameObject
{
private:
	ENEMY_STATE m_EnemyState = ENEMY_ATTACK;
	ENEMY_STATE m_StateKeep{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	D3DXVECTOR3 m_Top = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_RiskPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_RandomSeachPoint = D3DXVECTOR3(0.0f,0.0f,0.0f);

	Model* m_Model{};

	class Scene* m_Scene{};
	class Lock* m_Lock{};
	class Player* m_Player{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};

	static int m_EnemyCount;	// enemy���J�E���g
	int m_BomCount{};			// �����J�E���g
	int m_EnemyID{};			// enemy��ID
	int m_Health{};				// �̗�
	int m_OldHealth{};			// �̗͔�r�p
	int m_MissileCount{};		// �~�T�C���N�[���^�C��
	int m_SmokeCount{};			// �����o�����o
	int m_RandomTracking{};		// �v���C���[�̌��ɕt������
	int m_SeachCount{};
	int m_QuaAmountCount{};

	bool m_CrashFlg = false;		// �j�󔻕ʃt���O
	bool m_MissileShot = true;		// �~�T�C�����˃t���O
	bool m_PlayerStealth = false;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool EnemyView(D3DXVECTOR3 forward, float fieldOfViewRadians, float viewDistancee);//���E
	void EnemyHoming(D3DXVECTOR3 target,bool reverse);//�U��
	void EnemyStateChange(int number);//�X�e�[�g�ύX

	// �X�e�[�g�֐�
	void UpdateAttack();
	void UpdateEscape();
	void UpdateSeach();
	void UpdateGroundRisk();


	Lock* GetAddLock()
	{
		return m_Lock;
	}

	int GetMaxEnemy()
	{
		return m_EnemyCount;
	}

	int GetEnemyID()
	{
		return m_EnemyID;
	}

	void SetCrash(bool crash)
	{
		m_CrashFlg = crash;
	}

	bool GetCrash()
	{
		return m_CrashFlg;
	}

	void SetPlayerStealth(bool stealth)
	{
		m_PlayerStealth = stealth;
	}

	void HealthMinus(int minus)
	{
		m_Health -= minus;
	}

	int GetHealth()
	{
		return m_Health;
	}
};
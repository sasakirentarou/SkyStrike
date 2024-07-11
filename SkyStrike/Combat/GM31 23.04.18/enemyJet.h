#pragma once

#include "enemyManager.h"
#include "model.h"
#include <string>

#define ROTATION_AMOUNT		0.05 //�@�̂̉�]�� �����������������nan�ɂȂ�0.015
#define ROTATION_FRAME		3	 //��]�p�x

#define MISSILE_COOLDOWN	8	 //�~�T�C���̃N�[���_�E��

#define MAX_HEALTH			100	 //�ő�̗�
#define MAX_RANGE			2500 //�ő�ړ��͈�
#define MAX_ESCAPE			800  //�ő�P�ދ���
#define MAX_VIEW_LENGTH		1000 //�~�T�C�����ˋ���
#define RISK_RELEASE		500	 //�ė�����������x

enum ENEMY_STATE
{
	ENEMY_ATTACK,		// �U��
	ENEMY_ESCAPE,		// �ޔ�
	ENEMY_SEACH,		// �T��
	ENEMY_GROUNDRISK,	// �ė����
	ENEMY_OUT_OF_RANGE,	// �͈͊O
};

class EnemyJet : public EnemyManager
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

	static Model* m_Model;

	class Audio* m_KillSE{};

	class Scene* m_Scene{};
	class Lock* m_Lock{};
	class Tgt* m_Tgt{};
	class Jet* m_Jet{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};
	class Fire* m_Fire01{};
	class Fire* m_Fire02{};
	class CollisionBox* m_Collision{};

	int m_BomCount{};			// �����J�E���g
	int m_OldHealth{};			// �̗͔�r�p
	int m_SmokeCount{};			// �����o���Ԋu
	int m_RandomTracking{};		// �v���C���[�̌��ɕt������
	int m_SeachCount{};
	int m_QuaAmountCount{};

	float m_PlayerLength{};

	bool m_StateLock{};
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Debug();

	void MoveHoming(D3DXVECTOR3 target,bool reverse);//�ړ��U��
	void EnemyStateChange(int number);//�X�e�[�g�ύX

	// �X�e�[�g�֐�
	void Attack() override;		//�U��
	void Escape();				//�P��
	void Seach() override;		//�{��
	void GroundRisk();			//�n�ʉ��
	void OutOfRange();			//��s�͈�

	Lock* GetAddLock() { return m_Lock; }
	
	void SetStateLock(bool lock) { m_StateLock = lock; }
};
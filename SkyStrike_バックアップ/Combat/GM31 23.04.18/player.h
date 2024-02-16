#pragma once

#include "model.h"
#include "gameObject.h"

#define FRARE_TIME			30
#define MIN_SPEED			0.4

#define MAX_MISSILE			80
#define MAX_FLARE			5
#define MAX_STEALTH			100

#define STEALTH_DOWN_SPEED	0.1
#define STEALTH_UP_SPEED	0.15



class Player : public GameObject
{
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11ShaderResourceView* m_DissolveTexture = NULL;

	D3DXVECTOR3 m_EnemyPos{};
	D3DXVECTOR3 m_Top{};

	D3DXQUATERNION m_Endquat = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	class Audio* m_MinigunSE{};
	class Audio* m_MissileSE{};
	class Audio* m_LockOnSE{};
	class Audio* m_LockSetSE{};


	class Scene* m_Scene{};
	class LockOn* m_LockOn{};
	class Cross* m_Cross{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};

	//�e�X�g
	class Missile* missile00{};
	class Missile* missile01{};

	int m_EnemyID{};			//�GID�ۑ��p
	int m_OldEnemyID = 100;
	int m_MissileLockID{};		//�~�T�C�����ˎ��U��ID�ۑ��p
	int m_DeathCount{};
	int m_FlareCount{};
	int m_MissilePosChange{};	//���˃~�T�C��pos�ύX�p
	int m_MissileAmount{};		//�~�T�C����
	int m_FlareAmount{};		//�t���A��

	float m_fVec{};				//���݃X�s�[�h
	float m_LockOnType{};		//���b�N�I���ړ��p
	float m_Threshold{};		//�f�B�]���u�������l
	float m_StealthAmount{};	//�X�e���X�c��

	bool m_LockSetFlg{};		//���b�N�I�������t���O
	bool m_LockSetSeFlg{};		//���b�N�I������SE�t���O
	bool m_StealthModeFlg{};	//�X�e���X���[�h�t���O
	bool m_DeathFlg{};
	bool m_FlareFlg{};
	bool m_MissileSetFlg00{};	//�~�T�C�������t���O
	bool m_MissileSetFlg01{};
	bool m_LockOnFlg{};			//���b�N�I���J�n�t���O
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool PlayerView(float fieldOfViewRadians, float viewDistancee);
	void SwitchTarget();
	void ShotFlare();

	int GetLockEnemy()
	{
		return m_EnemyID;
	}

	int GetMisLockID()
	{
		return m_MissileLockID;
	}

	float GetSpeed()
	{
		return m_fVec;
	}

	bool GetDeath()
	{
		return m_DeathFlg;
	}

	int GetMissileAmount()
	{
		return m_MissileAmount;
	}

	int GetFlareAmount()
	{
		return m_FlareAmount;
	}

	float GetStealthAmount()
	{
		return m_StealthAmount;
	}

	//�e�X�g

};
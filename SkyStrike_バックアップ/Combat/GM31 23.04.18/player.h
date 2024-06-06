#pragma once

#include "model.h"
#include "gameObject.h"

#define ROLL_SPEED			0.030
#define PITCH_SPEED			0.015
#define YAW_SPEED			0.005

#define SPEED_UP			0.02
#define SPEED_DOWN			0.005

#define FRARE_TIME			30
#define MIN_SPEED			0.4

#define MAX_MISSILE			80	//�ʏ�~�T�C���e��
#define MAX_LONG			14	//�����O�~�T�C���e��
#define MAX_SHORT			15	//�V���[�g�~�T�C���e��
#define MAX_RAIL			10	//���[���K���e��
#define MAX_FLARE			5	//�t���A��
#define MAX_STEALTH			100	//�X�e���X�c��

#define STEALTH_DOWN_SPEED	0.1
#define STEALTH_UP_SPEED	0.15
#define STEALTH_MAX			100

//���ꕺ��
enum SpecialWeapon
{
	WEAPON_LONG_MISSILE,	//2�̓������b�N�I���ł��A�З͂�����
	WEAPON_SHORT_MISSILE,	//�U�����\�������A�З͂�����
	WEAPON_RAIL_GUN			//����������ɖ����A�c�e�������Ȃ�
};

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
	class Audio* m_LockOnSE{};
	class Audio* m_LockSetSE{};
	class Audio* m_StealthSE{};
	class Audio* m_MissileAlertSE{};

	class Scene* m_Scene{};
	class LockOn* m_LockOn{};
	class Cross* m_Cross{};
	class Trail* m_Trail01{};
	class Trail* m_Trail02{};
	class Lockon2D* m_Lockon2D{};
	class CollisionBox* m_Collision{};

	//�~�T�C��
	class Missile* m_Missile00{};
	class Missile* m_Missile01{};
	class LongMissile* m_LongMissile00{};
	class LongMissile* m_LongMissile01{};

	//class MissileUI* m_specialUI_0{};
	//class MissileUI* m_specialUI_1{};
	class LongMissileUI* m_LongUI_0{};
	class LongMissileUI* m_LongUI_1{};

	static SpecialWeapon m_SpecialWeapon;

	int m_EnemyID{};				//�GID�ۑ��p
	//std::vector<int> m_EnemyID{};				//�GID�ۑ��p
	int m_OldEnemyID = 100;
	int m_MissileLockID{};			//�~�T�C�����ˎ��U��ID�ۑ��p
	int m_DeathCount{};				//�ė��p
	int m_FlareCount{};				//�t���A�Ԋu�p
	int m_MissileVibCount{};		//�~�T�C���o�C�u���[�V�����p
	int m_MissilePosChange{};		//���˃~�T�C��pos�ύX�p
	int m_SpecialMissilePosChange{};//���ꕺ��pos�ύX�p
	int m_MissileAmount{};			//�~�T�C����
	int m_SpecialMissileAmount{};	//���ꕺ����
	int m_FlareAmount{};			//�t���A��

	float m_fVec{};				//���݃X�s�[�h
	float m_LockOnType{};		//���b�N�I���ړ��p
	float m_Threshold{};		//�f�B�]���u�������l
	float m_StealthAmount{};	//�X�e���X�c��

	bool m_LockSetFlg{};		//���b�N�I�������t���O
	bool m_LockSetSeFlg{};		//���b�N�I������SE�t���O
	bool m_LockSeFlg{};			//���b�N�I����SE�t���O
	bool m_StealthModeFlg{};	//�X�e���X���[�h�t���O
	bool m_DeathFlg{};			//�v���C���[���S�t���O
	bool m_FlareFlg{};			
	bool m_MissileSetFlg00{};			//�~�T�C�������t���O
	bool m_MissileSetFlg01{};
	bool m_SpecialMissileSetFlg00{};	//���ꕺ�������t���O
	bool m_SpecialMissileSetFlg01{};
	bool m_SpecialWeaponMode{};
	bool m_LockOnFlg{};					//���b�N�I���J�n�t���O
	bool m_RoleLock{};
	bool m_PitchLock{};
	bool m_YawLock{};
	bool m_EngineLock{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//�ʏ�~�T�C���֐�
	void UpdateMissile();
	void ShotMissile();
	
	//Weapon�X�e�[�g�֐�
	void UpdateLong();
	void UpdateShort();
	void UpdateRail();
	void ShotLong();
	void ShotShort();
	void ShotRail();

	//�����֐�
	bool PlayerView(float fieldOfViewRadians, float viewDistancee);
	void SwitchTarget();
	void ShotFlare();


	//�Q�b�^�[,�Z�b�^�[
	int GetLockEnemy() { return m_EnemyID; }
	int GetMisLockID() { return m_MissileLockID; }
	float GetSpeed() { return m_fVec; }
	bool GetDeath() { return m_DeathFlg; }
	bool GetWeaponChange() { return m_SpecialWeaponMode; }

	Audio* GetMissileAlertSE() { return m_MissileAlertSE; }
	LockOn* GetLockOn() { return m_LockOn; }

	int GetMissileAmount() { return m_MissileAmount; }
	int GetSpecialAmount() { return m_SpecialMissileAmount; }
	int GetFlareAmount() { return m_FlareAmount; }
	float GetStealthAmount() { return m_StealthAmount; }
	bool GetStealthFlg() { return m_StealthModeFlg; }
	bool GetFlareFlg() { return m_FlareFlg; }
	bool GetLockFlg() { return m_LockSetFlg; }

	//�@�\���b�N
	void SetRoleLock(bool lock) { m_RoleLock = lock; }
	void SetPitchLock(bool lock) { m_PitchLock = lock; }
	void SetYawLock(bool lock) { m_YawLock = lock; }
	void SetEngineLock(bool lock) { m_EngineLock = lock; }
	bool GetEngineLock() { return m_EngineLock; }
};
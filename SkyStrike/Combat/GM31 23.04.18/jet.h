#pragma once

#include "gameObject.h"
#include "model.h"

#include "engine.h"
#include "controlStick.h"
#include "weaponSystem.h"
#include "lockOnSystem.h"
#include "stealthSystem.h"
#include "flareSystem.h"
#include "wing.h"

#define SPEED_UP			0.02	//�����x
#define SPEED_DOWN			0.005	//�����x

#define MIN_SPEED			400		//�d�͂̉e�����󂯂�ŏ����xkm


#define TRAIL_WIDTH			0.02		//�g���C����

//�����͈�
#define MAIN_WING_RANGE		D3DX_PI / 8		//�嗃
#define BACK_WING_RANGE		D3DX_PI / 8		//����
#define VERTICAL_WING_RANGE	D3DX_PI / 20	//��������


struct Jet_State
{
	float Accele;
	float Brake;
	
	float Yaw;		
	float Pitch;
	float Roll;

	bool Control = true;
};



class Jet : public GameObject
{
private:
	Model* m_Model;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11ShaderResourceView* m_DissolveTexture = NULL;

	class Audio* m_StealthSE{};
	class Audio* m_AlertSE{};

	class Scene*		m_Scene{};
	class Trail*		m_Trail[2]{};
	class CollisionBox* m_Collision{};


	//�p�[�c
	Engine			m_Engine;
	ControlStick	m_ControlStick;

	WeaponSystem	m_WeaponSm;
	LockOnSystem	m_LockOnSm;
	StealthSystem	m_StealthSm;
	FlareSystem		m_FlareSm;

	Wing			m_WingFR; //�E�嗃
	Wing			m_WingFL; //���嗃
	Wing			m_WingBR; //�E����
	Wing			m_WingBL; //������
	Wing			m_WingTR; //�E��������
	Wing			m_WingTL; //����������

	
	
	Jet_State		m_JetState;

	
	D3DXVECTOR3 m_TopVertex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	float m_Speed{};					//���x


	int m_DeathCount{};					//�ė��p
	bool m_DeathFlg{};					//�v���C���[���S�t���O


	bool m_StealthModeFlg{};			//�X�e���X���[�h�t���O
	bool m_FlareFlg{};					//�t���A�N�[���_�E���p

	bool m_RoleLock{};					//���[�����샍�b�N
	bool m_PitchLock{};					//�s�b�`���샍�b�N
	bool m_YawLock{};					//���[���샍�b�N
	bool m_EngineLock{};				//�G���W�����샍�b�N


	//�V�F�[�_�[�p�����[�^
	bool m_SpecularEnable = false;		//�X�y�L�����t���O
	bool m_EnvEnable = true;			//���}�b�s���O�t���O
	bool m_LambertEnable = true;			//IBL�t���O
	bool m_IBLEnable = true;			//IBL�t���O
	float m_Shapness = 30.0f;			//�s��
	float m_Brightness = 1.0f;			//���x
	float m_Reflect = 0.05;				//���˓x
	float m_DissolveRange = 0.3f;		//�f�B�]���u�͈�
	D3DXCOLOR m_DissolveColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f); //�f�B�]���u�J���[

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Debug();


	float GetSpeed() { return m_Speed; }
	bool GetDeath() { return m_DeathFlg; }

	bool GetStealthFlg() { return m_StealthModeFlg; }
	bool GetFlareFlg() { return m_FlareFlg; }


	//���b�N
	void SetRoleLock(bool lock) { m_RoleLock = lock; }
	void SetPitchLock(bool lock) { m_PitchLock = lock; }
	void SetYawLock(bool lock) { m_YawLock = lock; }
	void SetEngineLock(bool lock) { m_EngineLock = lock; }
	bool GetEngineLock() { return m_EngineLock; }


	//�N���X�擾
	Audio* GetAlertSE() { return m_AlertSE; }
	WeaponSystem* GetWeaponSm() { return &m_WeaponSm; }
	LockOnSystem* GetLockOnSm() { return &m_LockOnSm; }
	StealthSystem* GetStealthSm() { return &m_StealthSm; }
	FlareSystem* GetFlareSm() { return &m_FlareSm; }
};
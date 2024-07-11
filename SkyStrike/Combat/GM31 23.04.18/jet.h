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

#define SPEED_UP			0.02	//Á¬x
#define SPEED_DOWN			0.005	//¸¬x

#define MIN_SPEED			400		//dÍÌe¿ðó¯éÅ¬¬xkm


#define TRAIL_WIDTH			0.02		//gC

//Â®ÍÍ
#define MAIN_WING_RANGE		D3DX_PI / 8		//å
#define BACK_WING_RANGE		D3DX_PI / 8		//ö
#define VERTICAL_WING_RANGE	D3DX_PI / 20	//¼ö


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


	//p[c
	Engine			m_Engine;
	ControlStick	m_ControlStick;

	WeaponSystem	m_WeaponSm;
	LockOnSystem	m_LockOnSm;
	StealthSystem	m_StealthSm;
	FlareSystem		m_FlareSm;

	Wing			m_WingFR; //Eå
	Wing			m_WingFL; //¶å
	Wing			m_WingBR; //Eö
	Wing			m_WingBL; //¶ö
	Wing			m_WingTR; //E¼ö
	Wing			m_WingTL; //¶¼ö

	
	
	Jet_State		m_JetState;

	
	D3DXVECTOR3 m_TopVertex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	float m_Speed{};


	int m_DeathCount{};					//Äp
	bool m_DeathFlg{};					//vC[StO


	bool m_StealthModeFlg{};			//XeX[htO
	bool m_FlareFlg{};					//tAN[_Ep

	bool m_RoleLock{};					//[ìbN
	bool m_PitchLock{};					//sb`ìbN
	bool m_YawLock{};					//[ìbN
	bool m_EngineLock{};				//GWìbN
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


	//bN
	void SetRoleLock(bool lock) { m_RoleLock = lock; }
	void SetPitchLock(bool lock) { m_PitchLock = lock; }
	void SetYawLock(bool lock) { m_YawLock = lock; }
	void SetEngineLock(bool lock) { m_EngineLock = lock; }
	bool GetEngineLock() { return m_EngineLock; }


	//NXæ¾
	Audio* GetAlertSE() { return m_AlertSE; }
	WeaponSystem* GetWeaponSm() { return &m_WeaponSm; }
	LockOnSystem* GetLockOnSm() { return &m_LockOnSm; }
	StealthSystem* GetStealthSm() { return &m_StealthSm; }
	FlareSystem* GetFlareSm() { return &m_FlareSm; }
};